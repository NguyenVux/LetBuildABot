#include "raylib.h"
#include <cassert>
#include <format>
#include <memory>
#include <vector>
#include <deque>
#include <stack>
#include <algorithm>
#include <UI/Widget.h>
#include <UI/UIManager.h>


using namespace UI;


void DrawTest()
{
    constexpr int screenWidth = 640;
    constexpr int screenHeight = 640;

    InitWindow(screenWidth, screenHeight, "LetBuildAbot2");
    SetTargetFPS(60);

    UIManager manager;
    // manager.root = ;
    manager.GetRootWidget()->SetName("root");
    std::unique_ptr<VContainerWidget> c1= std::make_unique<VContainerWidget>(manager.GetRootWidget());
    c1->rect.x = 0;
    c1->rect.y = 20;
    c1->border = 0.0f;
    c1->spacing = 20.0f;
    c1->rect.width = 80.0f + c1->border * 2.0f;

    c1->SetName("VContainerWidget");
    // c1->GetParent() = manager.root.get();
    manager.GetRootWidget()->m_children.push_back(c1.get());
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
        blocks[i]->SetName(std::format("Block name {}", i));
    }

    c1->UpdateLayout();

    std::unique_ptr<VContainerWidget> c2= std::make_unique<VContainerWidget>(manager.GetRootWidget());
    c2->border = 0.0f;
    c2->spacing = 20.0f;
    c2->rect.width = 80.0f + c1->border * 2.0f;
    c2->rect.x = c1->rect.x + c1->rect.width - 50.0f;
    c2->rect.y = 30;
    c2->SetName("VContainerWidget2");
    // c2->GetParent() = manager.root.get();
    // c2->color = BLUE;
    manager.GetRootWidget()->m_children.push_back(c2.get());
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
        blocks2[i]->SetName(std::format("Block name {}", i));
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
    DrawTest();
}
