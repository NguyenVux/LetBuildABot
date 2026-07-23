#include "Instruction/Instruction.h"
#include "Player.h"
#include "Runner.h"
#include "raylib.h"
#include "raymath.h"
#include <cassert>
#include <cstddef>
#include <format>
#include <memory>
#include <queue>
#include <vector>

void DrawInfiniteGrid(const Camera2D& camera, int screenWidth, int screenHeight);

class Widget
{
public:
    std::string name;
    std::vector<Widget*> m_children;
    bool Visible = true;
    bool Hover = false;
    Rectangle rect;
    virtual void UpdateLayout() {};
    virtual void OnMouseEnter() {};
    virtual void OnMouseLeave() {};
    virtual void OnMouseDown() {};
    virtual void OnMouseUp() {};
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

    virtual void OnMouseEnter() override {
        TraceLog(LOG_INFO, std::format("Mouse Enter VContainer").c_str());
    }
    virtual void OnMouseLeave() override
    {
        TraceLog(LOG_INFO, std::format("Mouse Leave VContainer").c_str());
    }
};
class UIManager
{
public:
    std::unique_ptr<Widget> root = nullptr;
    Widget* currentInteraction = nullptr;

    void Update()
    {
        Vector2 mousePos = GetMousePosition();
        bool isLMousePressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        std::queue<Widget*> widgetsToDraw;
        widgetsToDraw.push(root.get());
        while (!widgetsToDraw.empty())
        {
            Widget* top = widgetsToDraw.front();
            widgetsToDraw.pop();
            if (!top)
            {
                continue;
            }
            if (!top->Visible)
            {
                continue;
            }
            bool isHover = CheckCollisionPointRec(mousePos,top->rect);
            if(isHover != top->Hover)
            {
                top->Hover = isHover;
                if(top->Hover)
                {
                    top->OnMouseEnter();
                }
                else {
                    top->OnMouseLeave();
                }
            }
            
             

            for (Widget* child : top->m_children)
            {
                widgetsToDraw.push(child);            
            }
        }
    }
    void Draw()
    {
        std::queue<Widget*> widgetsToDraw;
        widgetsToDraw.push(root.get());
        while (!widgetsToDraw.empty())
        {
            const Widget* top = widgetsToDraw.front();
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
        Color color = Hover?RED:GREEN;
        // }
        DrawRectangleRec(rect, color);
    }
    virtual void OnMouseEnter() override {
       TraceLog(LOG_INFO, "Mouse enter %s", name.c_str());
    }

    virtual void OnMouseLeave() override {
       TraceLog(LOG_INFO, "Mouse leave %s", name.c_str());
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
        manager.root->m_children.push_back(&block);
    }
    
    for(int i = 0; i < blocks.size(); i++)
    {
        blocks[i].name = std::format("Block name {}",i);
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