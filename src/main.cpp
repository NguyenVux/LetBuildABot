#include "raylib.h"
#include <cstdint>
#include <functional>
#include <vector>

constexpr float s_tileSize = 64.0f;

void DrawInfiniteGrid(const Camera2D& camera, int screenWidth, int screenHeight);
class Player;
void Move(Player& player);
void Rotate(Player& player, int direction);

enum class Facing : uint8_t
{
	Right,
	Down,
	Left,
	Up,
	Count,
};

class Player
{
public:
	uint32_t function_pointer = 0;
	Vector2 position = {0.0f, 0.0f};
	Vector2 size = {1.0f, 1.0f};
	Facing facing = Facing::Right;
	std::vector<std::function<void(Player&)>> instructionList;
public:
	Player(const std::string& texturePath)
	{
		texture = LoadTexture(texturePath.c_str());
	}

	~Player()
	{
		UnloadTexture(texture);
	}

	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

	bool Update()
	{
		if (function_pointer >= instructionList.size())
		{
			return false;
		}

		instructionList.at(function_pointer)(*this);
		function_pointer++;
		return function_pointer < instructionList.size();
	}
	void Draw() const
	{
		const Vector2 pixelSize = Vector2Scale(size, s_tileSize);
		const Rectangle source = {0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)};
		const Rectangle destination = {
			position.x,
			position.y,
			pixelSize.x,
			pixelSize.y,
		};
		DrawTexturePro(texture, source, destination, Vector2Scale(pixelSize, 0.5f), (int)facing * 90.0f, WHITE);
	}

private:
	Texture2D texture = {};
};

void Move(Player& player)
{
	switch (player.facing)
	{
	case Facing::Right:
		player.position.x += s_tileSize;
		break;
	case Facing::Down:
		player.position.y += s_tileSize;
		break;
	case Facing::Left:
		player.position.x -= s_tileSize;
		break;
	case Facing::Up:
		player.position.y -= s_tileSize;
		break;
	default:
		break;
	}
}

void Rotate(Player& player, int direction)
{
	const int facingCount = static_cast<int>(Facing::Count);
	const int currentFacing = static_cast<int>(player.facing);
	const int nextFacing = (currentFacing + direction + facingCount) % facingCount;

	player.facing = static_cast<Facing>(nextFacing);
}

int main()
{
	constexpr int screenWidth = 640;
	constexpr int screenHeight = 640;

	InitWindow(screenWidth, screenHeight, "LetBuildAbot");
	SetTargetFPS(60);

	Player player("assets/arrow.png");
	player.instructionList.push_back(std::bind(Move, std::placeholders::_1));
	player.instructionList.push_back(std::bind(Move, std::placeholders::_1));
	player.instructionList.push_back(std::bind(Move, std::placeholders::_1));
	player.instructionList.push_back(std::bind(Rotate, std::placeholders::_1, -1));
	player.instructionList.push_back(std::bind(Move, std::placeholders::_1));
	player.instructionList.push_back(std::bind(Move, std::placeholders::_1));
	player.instructionList.push_back(std::bind(Rotate, std::placeholders::_1, 1));

	Camera2D camera = {};
	camera.offset = {screenWidth * 0.5f, screenHeight * 0.5f};
	camera.target = player.position;
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	constexpr float instructionDelay = 0.5f;
	float instructionTimer = 0.0f;

	while (!WindowShouldClose())
	{
		camera.target = player.position;
		instructionTimer += GetFrameTime();
		if (instructionTimer >= instructionDelay)
		{
			instructionTimer = 0.0f;
			if(!player.Update())
			{
				TraceLog(LOG_INFO, "Done executing");
			}
		}
		BeginDrawing();
		ClearBackground(BLACK);

		BeginMode2D(camera);
		DrawInfiniteGrid(camera, screenWidth, screenHeight);
		player.Draw();
		EndMode2D();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
