#include "Player.h"
#include "Instruction/Instruction.h"
#include "Runner.h"
#include <format>

void DrawInfiniteGrid(const Camera2D& camera, int screenWidth, int screenHeight)
{
    constexpr float axisThickness = 2.0f;

    const Vector2 topLeft = GetScreenToWorld2D({0.0f, 0.0f}, camera);
    const Vector2 bottomRight = GetScreenToWorld2D(
        {static_cast<float>(screenWidth), static_cast<float>(screenHeight)}, camera);

    const int startX = static_cast<int>(topLeft.x / s_tileSize) - 1;
    const int endX = static_cast<int>(bottomRight.x / s_tileSize) + 1;
    const int startY = static_cast<int>(topLeft.y / s_tileSize) - 1;
    const int endY = static_cast<int>(bottomRight.y / s_tileSize) + 1;

    for (int x = startX; x <= endX; ++x)
    {
        const float worldX = static_cast<float>(x) * s_tileSize;
        const Color color = (x == 0) ? GRAY : DARKGRAY;
        const float thickness = (x == 0) ? axisThickness : 1.0f;
        DrawLineEx({worldX, topLeft.y - s_tileSize}, {worldX, bottomRight.y + s_tileSize},
                   thickness, color);
    }

    for (int y = startY; y <= endY; ++y)
    {
        const float worldY = static_cast<float>(y) * s_tileSize;
        const Color color = (y == 0) ? GRAY : DARKGRAY;
        const float thickness = (y == 0) ? axisThickness : 1.0f;
        DrawLineEx({topLeft.x - s_tileSize, worldY}, {bottomRight.x + s_tileSize, worldY},
                   thickness, color);
    }
}

void RunPlayerInstructionTest(int screenWidth, int screenHeight)
{
    Player player("assets/arrow.png");
    State state = {
        player,
    };
    MoveInstruction move;
    MoveInstruction move2;
    MoveInstruction move3;
    RotateInstruction rot(90.0f);
    DelayInstruction delay(0.5f);
    DelayInstruction delay2(0.5f);
    DelayInstruction delay3(0.5f);
    DelayInstruction delay4(0.5f);
    RotateInstruction rot2(-90.0f);

    move.SetNext(&delay);
    delay.SetNext(&rot);
    rot.SetNext(&delay2);
    delay2.SetNext(&move2);
    move2.SetNext(&delay3);
    delay3.SetNext(&rot2);
    rot2.SetNext(&delay4);
    delay4.SetNext(&move3);
    move3.SetNext(nullptr);

    Camera2D camera = {};
    camera.offset = {screenWidth * 0.5f, screenHeight * 0.5f};
    camera.target = player.position;
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Instruction* instruction = &move;
    instruction->Setup(state);

    while (!WindowShouldClose())
    {
        camera.target = player.position;

        BeginDrawing();
        ClearBackground(BLACK);

        if (instruction != nullptr)
        {
            if (!instruction->isDone(state))
            {
                instruction->Execute(state);
            }
            else
            {
                instruction = instruction->Next();
                if (instruction != nullptr)
                {
                    instruction->Setup(state);
                }
            }
        }

        BeginMode2D(camera);
        DrawInfiniteGrid(camera, screenWidth, screenHeight);
        player.Draw();
        EndMode2D();

        if (instruction == nullptr)
        {
            DrawText("Done", 0, 0, 24, RAYWHITE);
        }
        else
        {
            MoveInstruction* moveInstruction = dynamic_cast<MoveInstruction*>(instruction);
            if (moveInstruction != nullptr)
            {
                DrawText(std::format("Current Position: {} \n"
                                     "Target Position: {} \n"
                                     "Distance: \n {}",
                                     player.position.x, moveInstruction->m_end.x,
                                     Vector2Distance(moveInstruction->m_end, player.position))
                             .c_str(),
                         0, 0, 24, RAYWHITE);
            }
        }

        EndDrawing();
    }
}