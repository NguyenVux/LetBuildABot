#include "raylib.h"
#include "raymath.h"

namespace
{
void DrawInfiniteGrid(const Camera2D& camera, int screenWidth, int screenHeight)
{
    constexpr float gridSize = 64.0f;
    constexpr float axisThickness = 2.0f;

    const Vector2 topLeft = GetScreenToWorld2D({0.0f, 0.0f}, camera);
    const Vector2 bottomRight = GetScreenToWorld2D({static_cast<float>(screenWidth), static_cast<float>(screenHeight)}, camera);

    const int startX = static_cast<int>(topLeft.x / gridSize) - 1;
    const int endX = static_cast<int>(bottomRight.x / gridSize) + 1;
    const int startY = static_cast<int>(topLeft.y / gridSize) - 1;
    const int endY = static_cast<int>(bottomRight.y / gridSize) + 1;

    for (int x = startX; x <= endX; ++x)
    {
        const float worldX = static_cast<float>(x) * gridSize;
        const Color color = (x == 0) ? GRAY : DARKGRAY;
        const float thickness = (x == 0) ? axisThickness : 1.0f;
        DrawLineEx({worldX, topLeft.y - gridSize}, {worldX, bottomRight.y + gridSize}, thickness, color);
    }

    for (int y = startY; y <= endY; ++y)
    {
        const float worldY = static_cast<float>(y) * gridSize;
        const Color color = (y == 0) ? GRAY : DARKGRAY;
        const float thickness = (y == 0) ? axisThickness : 1.0f;
        DrawLineEx({topLeft.x - gridSize, worldY}, {bottomRight.x + gridSize, worldY}, thickness, color);
    }
}
}

int main()
{
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;
    constexpr float moveSpeed = 280.0f;

    InitWindow(screenWidth, screenHeight, "LetBuildAbot");
    SetTargetFPS(60);

    Texture2D arrowTexture = LoadTexture("assets/arrow.png");
    Vector2 arrowPosition = {0.0f, 0.0f};

    Camera2D camera = {};
    camera.offset = {screenWidth * 0.5f, screenHeight * 0.5f};
    camera.target = arrowPosition;
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();
        Vector2 moveDirection = {0.0f, 0.0f};

        if (IsKeyDown(KEY_W)) moveDirection.y -= 1.0f;
        if (IsKeyDown(KEY_S)) moveDirection.y += 1.0f;
        if (IsKeyDown(KEY_A)) moveDirection.x -= 1.0f;
        if (IsKeyDown(KEY_D)) moveDirection.x += 1.0f;

        if (moveDirection.x != 0.0f || moveDirection.y != 0.0f)
        {
            moveDirection = Vector2Normalize(moveDirection);
            arrowPosition.x += moveDirection.x * moveSpeed * deltaTime;
            arrowPosition.y += moveDirection.y * moveSpeed * deltaTime;
        }

        camera.target = arrowPosition;

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
        DrawInfiniteGrid(camera, screenWidth, screenHeight);

        const Rectangle source = {0.0f, 0.0f, static_cast<float>(arrowTexture.width), static_cast<float>(arrowTexture.height)};
        const Rectangle destination = {
            arrowPosition.x,
            arrowPosition.y,
            static_cast<float>(arrowTexture.width),
            static_cast<float>(arrowTexture.height)
        };
        const Vector2 origin = {arrowTexture.width * 0.5f, arrowTexture.height * 0.5f};
        DrawTexturePro(arrowTexture, source, destination, origin, 0.0f, WHITE);

        EndMode2D();

        DrawText("WASD", 16, 16, 24, RAYWHITE);

        EndDrawing();
    }

    UnloadTexture(arrowTexture);
    CloseWindow();

    return 0;
}
