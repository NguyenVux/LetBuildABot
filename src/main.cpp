#include "Instruction/Instruction.h"
#include "Player.h"
#include "Runner.h"
#include "raylib.h"
#include "raymath.h"
#include <cassert>
#include <csetjmp>
#include <cstddef>
#include <format>
#include <iterator>
#include <memory>
#include <stack>
#include <tuple>
#include <vector>
#include <deque>
#include <algorithm>

void DrawInfiniteGrid(const Camera2D& camera, int screenWidth, int screenHeight);

class Widget
{
    public:
    std::string name;
    std::vector<Widget*> m_children;
    bool Visible = true;
    bool Hover = false;
    Rectangle rect;
    int depth;
    Widget* parent = nullptr; 
    
    Widget(Widget* parent): 
        parent(parent)
    {
        depth = 0;
        if(parent)
        {
           depth = parent->depth + 1; 
        }
    }
    //===================Static Functions========================
    bool isAncestorOf(const Widget* widget) const
    {
        return widget->isDescendanceOf(this);
    }

    bool isDescendanceOf(const Widget* widget) const
    {
        const Widget* currentParent = parent;
        while(currentParent != nullptr)
        {
           if(currentParent == widget) 
           {
                return true;
           }
           currentParent = currentParent->parent;
        }
        return false;
    }
    //===================Event Functions=========================
    virtual bool OnMouseEnter() { return false; }
    virtual bool OnMouseLeave() { return false; }
    virtual bool OnMouseDown() { 
        TraceLog(LOG_INFO, "Mouse Down %s", name.c_str());
        return false;
    }
    virtual bool OnMouseUp() { return false; }
    //===========================================================
     
    virtual void UpdateLayout() { }
    virtual void Draw() const { }
    virtual ~Widget()
    {
    }
};


class VContainerWidget : public Widget
{
    public:
    float spacing = 5.0f;
    float border = 5.0f;
    
    Color color = {89, 89, 89, 255};
    VContainerWidget(Widget* parent): Widget(parent)
    {

    }

    virtual void UpdateLayout() override
    {
        rect.height = border * 2.0f;
        int realChildCount = 0;
        for (int i = 0; i < m_children.size(); i++)
        {
            Widget* child = m_children[i];
            if (child == nullptr)
            {
                TraceLog(LOG_WARNING, "%s::UpdateLayout() child index %d null", name.c_str(), i);
                continue;
            }
            realChildCount++;
            child->rect.x = rect.x + border;
            child->rect.y = rect.y + border;
            if (i > 0)
            {
                Widget* prevChild = nullptr;
                int prev = i;
                do
                {
                    prevChild = m_children[--prev];
                } while (prevChild == nullptr && prev > 0);
                if (prevChild != nullptr)
                {
                    child->rect.y = prevChild->rect.y + prevChild->rect.height + spacing;
                }
            }
            rect.height += child->rect.height;
        }
        rect.height += (realChildCount - 1.0f) * spacing;
    }

    virtual void Draw() const override
    {
        DrawRectangleRec(rect, color);
    }
};
class UIManager
{
    public:
    std::unique_ptr<Widget> root = nullptr;
    Widget* currentInteraction = nullptr;
    Widget* lastHittedWidget = nullptr;

    bool prevMouseState = 0x0;

    Widget* Raycast()
    {
        Vector2 mousePos = GetMousePosition();
        Widget* hitted = nullptr;
        std::stack<Widget*> widgets;
        widgets.push(root.get());
        while (!widgets.empty())
        {
            Widget* top = widgets.top();
            widgets.pop();
            if (!top)
            {
                continue;
            }
            if (!top->Visible)
            {
                continue;
            }
            bool isHit = CheckCollisionPointRec(mousePos, top->rect);
            if(isHit)
            {
                hitted = top;
            }

            for (auto it = top->m_children.rbegin(); it != top->m_children.rend(); it++)
            {

                widgets.push(*it);
            }
        }

        return hitted;
    }
    

    std::deque<Widget*> getTreePath(Widget* widget)
    {
        std::deque<Widget*> widgetTree;
        while(widget != nullptr)
        {
            widgetTree.push_front(widget);
            widget = widget->parent;
        }
        return widgetTree;
    }
    
    

    const Widget* GetCommonNode(
        const Widget* node1,
        const Widget* node2)
    {
        if (!node1 || !node2)
        {
            return nullptr;
        }

        if (node1->depth < node2->depth)
        {
            std::swap(node1, node2);
        }

        while (node1->depth > node2->depth)
        {
            node1 = node1->parent;
        }

        while (node1 != node2)
        {
            node1 = node1->parent;
            node2 = node2->parent;

            if (!node1 || !node2)
            {
                return nullptr;
            }
        }

        return node1;
    }
    void Update()
    {
        Widget* hitted = Raycast();
        

        std::stack<Widget*> fromRoot;

        std::deque<Widget*> widgetTree = getTreePath(hitted);
        std::deque<Widget*> capturedTree = getTreePath(lastHittedWidget);
        

        //Get common ancestor

        auto commonNode = GetCommonNode(hitted, lastHittedWidget);
        Widget* currentMouseLeave = lastHittedWidget;
        Widget* currentMouseEnter = hitted;

        while(currentMouseLeave != commonNode)
        {
            TraceLog(LOG_INFO, "Mouse Leave: %s",currentMouseLeave->name.c_str());
            currentMouseLeave->OnMouseLeave();
            currentMouseLeave = currentMouseLeave->parent;
        }

        while(currentMouseEnter != commonNode)
        {
            TraceLog(LOG_INFO, "Mouse Enter: %s",currentMouseEnter->name.c_str());
            currentMouseEnter->OnMouseEnter();
            currentMouseEnter = currentMouseEnter->parent;
        }

        lastHittedWidget = hitted;
    }

    
    void Draw()
    {
        std::stack<Widget*> widgetsToDraw;
        widgetsToDraw.push(root.get());
        while (!widgetsToDraw.empty())
        {
            const Widget* top = widgetsToDraw.top();
            widgetsToDraw.pop();
            if (!top)
            {
                continue;
            }
            if (!top->Visible)
            {
                continue;
            }
            top->Draw();
            for (auto it = top->m_children.rbegin(); it != top->m_children.rend(); it++)
            {
                Widget* child = *it;
                widgetsToDraw.push(child);
            }
        }
    }
};

class Block : public Widget
{
    public:
    Color color = GREEN;
    bool isHover = false;
    Block(Widget* parent): Widget(parent)
    {

    }
    virtual void Draw() const override
    {
        Color _color = isHover?RED:GREEN;
        DrawRectangleRec(rect, _color);
    }
    virtual bool OnMouseEnter() override
    {
        isHover = true;
        // TraceLog(LOG_INFO, "Mouse enter %s", name.c_str());
        return false;
    }

    virtual bool OnMouseLeave() override
    {
        isHover = false;
        // TraceLog(LOG_INFO, "Mouse leave %s", name.c_str());
        return false;
    }

    virtual bool OnMouseDown() override
    {
        TraceLog(LOG_INFO, "Mouse down %s", name.c_str());
        return false;
    }
};

void DrawTest()
{
    constexpr int screenWidth = 640;
    constexpr int screenHeight = 640;

    InitWindow(screenWidth, screenHeight, "LetBuildAbot");
    SetTargetFPS(60);

    UIManager manager;
    manager.root = std::make_unique<Widget>(nullptr);
    manager.root->name = "root";
    std::unique_ptr<VContainerWidget> c1= std::make_unique<VContainerWidget>(manager.root.get());
    c1->rect.x = 0;
    c1->rect.y = 20;
    c1->border = 0.0f;
    c1->spacing = 20.0f;
    c1->rect.width = 80.0f + c1->border * 2.0f;

    c1->name = "VContainerWidget";
    c1->parent = manager.root.get();
    manager.root->m_children.push_back(c1.get());
    std::vector<std::unique_ptr<Block>> blocks;
    constexpr int blockCount = 5;
    blocks.reserve(blockCount);
    for(int i = 0; i < blockCount; i++)
    {
        auto& block = blocks.emplace_back(std::make_unique<Block>(c1.get()));
        block->rect.width = 80;
        block->rect.height = 30;
        c1->m_children.push_back(block.get());
    }

    for (int i = 0; i < blocks.size(); i++)
    {
        blocks[i]->name = std::format("Block name {}", i);
    }

    c1->UpdateLayout();

    std::unique_ptr<VContainerWidget> c2= std::make_unique<VContainerWidget>(manager.root.get());
    c2->border = 0.0f;
    c2->spacing = 20.0f;
    c2->rect.width = 80.0f + c1->border * 2.0f;
    c2->rect.x = c1->rect.x + c1->rect.width - 50.0f;
    c2->rect.y = 30;
    c2->name = "VContainerWidget2";
    c2->parent = manager.root.get();
    c2->color = BLUE;
    manager.root->m_children.push_back(c2.get());
    std::vector<std::unique_ptr<Block>> blocks2;
    blocks2.reserve(blockCount);

    for(int i = 0; i < blockCount; i++)
    {
        auto& block = blocks2.emplace_back(std::make_unique<Block>(c2.get()));
        block->rect.width = 80;
        block->rect.height = 30;
        block->color = RED;
        c2->m_children.push_back(block.get());
    }

    for (int i = 0; i < blocks2.size(); i++)
    {
        blocks2[i]->name = std::format("Block name {}", i);
    }
    c2->UpdateLayout();

    while (!WindowShouldClose())
    {
        manager.Update();
        BeginDrawing();
        ClearBackground(BLACK);

        manager.Draw();
        EndDrawing();
    }

    CloseWindow();
}

int main()
{
    // raylib_example();
    DrawTest();
}
