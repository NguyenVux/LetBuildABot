#include "Instruction/Instruction.h"
#include "Player.h"
#include "Runner.h"
#include "raylib.h"
#include "raymath.h"
#include <cassert>
#include <cstddef>
#include <format>
#include <iterator>
#include <memory>
#include <stack>
#include <vector>
#include <deque>

void DrawInfiniteGrid(const Camera2D& camera, int screenWidth, int screenHeight);

class Widget
{
    public:
    std::string name;
    std::vector<Widget*> m_children;
    bool Visible = true;
    bool Hover = false;
    Rectangle rect;
    Widget* parent = nullptr; 
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
    virtual void Draw() const = 0;
    virtual ~Widget()
    {
    }
};

class VContainerWidget : public Widget
{
    public:
    float spacing = 5.0f;
    float border = 5.0f;

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
            child->rect.x = rect.x;
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
        Color color = {89, 89, 89, 255};
        DrawRectangleRec(rect, color);
    }
};
class UIManager
{
    public:
    std::unique_ptr<Widget> root = nullptr;
    Widget* currentInteraction = nullptr;

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

            for (Widget* child : top->m_children)
            {
                widgets.push(child);
            }
        }

        return hitted;
    }

    void Update()
    {
        Widget* hitted = Raycast();
        std::stack<Widget*> fromRoot;
        std::deque<Widget*> widgetTree;
        if(!hitted) return;

        while(hitted != nullptr)
        {
            widgetTree.push_front(hitted);
            hitted = hitted->parent;
        }
        bool isMouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        if(isMouseDown)
        {
            for(auto i = widgetTree.rbegin(); i != widgetTree.rend(); i++)
            {
                Widget* current = *i;            
                bool isConsumed = current->OnMouseDown();
                if(isConsumed)
                {
                    break;
                }
            }
        }
        std::string path;
        for(auto i = widgetTree.begin(); i != widgetTree.end(); i++)
        {
            Widget* current = *i;            
            if(!current->Hover)
            {
                
            }
            path += current->name;
            if(std::next(i) != widgetTree.end())
            {
               path += "/" ;
            }
            
        }
        TraceLog(LOG_INFO, path.c_str());
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
            for (Widget* child : top->m_children)
            {
                widgetsToDraw.push(child);
            }
        }
    }
};

class Block : public Widget
{
    public:
    virtual void Draw() const override
    {
        Color color = Hover ? RED : GREEN;
        // }
        DrawRectangleRec(rect, color);
    }
    virtual bool OnMouseEnter() override
    {
        TraceLog(LOG_INFO, "Mouse enter %s", name.c_str());
        return false;
    }

    virtual bool OnMouseLeave() override
    {
        TraceLog(LOG_INFO, "Mouse leave %s", name.c_str());
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
    manager.root = std::make_unique<VContainerWidget>();
    manager.root->rect.y = 20;
    manager.root->name = "root_VContainerWidget";
    std::vector<Block> blocks(5);
    for (Block& block : blocks)
    {
        block.rect.width = 80;
        block.rect.height = 30;
        block.parent = manager.root.get();
        manager.root->m_children.push_back(&block);
    }

    for (int i = 0; i < blocks.size(); i++)
    {
        blocks[i].name = std::format("Block name {}", i);
    }

    manager.root->UpdateLayout();
    manager.root->rect.width = 100;
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
