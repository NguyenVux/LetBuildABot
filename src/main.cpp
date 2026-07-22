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

    void Update()
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
        Vector2 mousePos = GetMousePosition();
        // bool hover = CheckCollisionPointRec(mousePos, rect);
        Color color = Hover?RED:GREEN;
        // }
        DrawRectangleRec(rect, color);
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
    std::vector<Block> blocks(15);
    for (Block& block : blocks)
    {
        block.rect.width = 80;
        block.rect.height = 30;
        manager.root->m_children.push_back(&block);
    }
    
    manager.root->UpdateLayout();
    manager.root->rect.width = 100;
    while (!WindowShouldClose())
    {
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