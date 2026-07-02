#include <stdint.h>
#define RAYMATH_IMPLEMENTATION
#include "raylib.h"
#include "raymath.h"
#include <math.h>


constexpr float s_tileSize = 64.0f;
enum class Facing : uint8_t
{
	RIGHT,
	DOWN,
	LEFT,
	UP,
	COUNT
};
class Player
{
public:
	Texture2D texture = {};
	Vector2 position = {0.0f, 0.0f};
	Vector2 size = {1.0f, 1.0f};
	float moveSpeed = 280.0f;
	Facing facing = Facing::RIGHT;
};

namespace
{
void DrawInfiniteGrid(const Camera2D& camera, int screenWidth, int screenHeight)
{
	constexpr float axisThickness = 2.0f;

	const Vector2 topLeft = GetScreenToWorld2D({0.0f, 0.0f}, camera);
	const Vector2 bottomRight = GetScreenToWorld2D({static_cast<float>(screenWidth), static_cast<float>(screenHeight)}, camera);

	const int startX = static_cast<int>(topLeft.x / s_tileSize) - 1;
	const int endX = static_cast<int>(bottomRight.x / s_tileSize) + 1;
	const int startY = static_cast<int>(topLeft.y / s_tileSize) - 1;
	const int endY = static_cast<int>(bottomRight.y / s_tileSize) + 1;

	for (int x = startX; x <= endX; ++x)
	{
		const float worldX = static_cast<float>(x) * s_tileSize;
		const Color color = (x == 0) ? GRAY : DARKGRAY;
		const float thickness = (x == 0) ? axisThickness : 1.0f;
		DrawLineEx({worldX, topLeft.y - s_tileSize}, {worldX, bottomRight.y + s_tileSize}, thickness, color);
	}

	for (int y = startY; y <= endY; ++y)
	{
		const float worldY = static_cast<float>(y) * s_tileSize;
		const Color color = (y == 0) ? GRAY : DARKGRAY;
		const float thickness = (y == 0) ? axisThickness : 1.0f;
		DrawLineEx({topLeft.x - s_tileSize, worldY}, {bottomRight.x + s_tileSize, worldY}, thickness, color);
	}
}
}

int main()
{
	constexpr int screenWidth = 640;
	constexpr int screenHeight = 640;

	InitWindow(screenWidth, screenHeight, "LetBuildAbot");
	SetTargetFPS(60);

	Player player;
	player.texture = LoadTexture("assets/arrow.png");

	Camera2D camera = {};
	camera.offset = {screenWidth * 0.5f, screenHeight * 0.5f};
	camera.target = player.position;
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	while (!WindowShouldClose())
	{
		const float deltaTime = GetFrameTime();
		const float time = GetTime();
		Vector2 moveDirection = {0.0f, 0.0f};

		if (IsKeyDown(KEY_W)) moveDirection.y -= 1.0f;
		if (IsKeyDown(KEY_S)) moveDirection.y += 1.0f;
		if (IsKeyDown(KEY_A)) moveDirection.x -= 1.0f;
		if (IsKeyDown(KEY_D)) moveDirection.x += 1.0f;
		if (IsKeyPressed(KEY_R))
		{
			player.facing = static_cast<Facing>(((uint8_t)player.facing + 1) % (uint8_t)Facing::COUNT);

		}

		if (moveDirection.x != 0.0f || moveDirection.y != 0.0f)
		{
			moveDirection = Vector2Normalize(moveDirection);
			player.position.x += moveDirection.x * player.moveSpeed * deltaTime;
			player.position.y += moveDirection.y * player.moveSpeed * deltaTime;
		}

		camera.target = player.position;
		BeginDrawing();
		ClearBackground(BLACK);

		BeginMode2D(camera);
		DrawInfiniteGrid(camera, screenWidth, screenHeight);
		Vector2 pixelSize = Vector2Scale(player.size, s_tileSize);
		const Rectangle source = {0.0f, 0.0f, static_cast<float>(player.texture.width), static_cast<float>(player.texture.height)};
		const Rectangle destination = {
			player.position.x,
			player.position.y,
			pixelSize.x,
			pixelSize.y,
		};
		const Vector2 origin = {player.texture.width * 0.5f, player.texture.height * 0.5f};
		DrawTexturePro(player.texture, source, destination, Vector2Scale(pixelSize, 0.5f), (float)player.facing * 90.0f, WHITE);

		EndMode2D();

		DrawText("WASD", 16, 16, 24, RAYWHITE);

		EndDrawing();
	}

	UnloadTexture(player.texture);
	CloseWindow();

	return 0;
}
