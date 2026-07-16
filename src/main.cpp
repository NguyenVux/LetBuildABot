#include "Instruction/Instruction.h"
#include "raylib.h"
#include "Player.h"
#include "Runner.h"
#include "raymath.h"
#include <cstddef>
#include <format>

void DrawInfiniteGrid(const Camera2D& camera, int screenWidth, int screenHeight);

int main()
{
	constexpr int screenWidth = 640;
	constexpr int screenHeight = 640;

	InitWindow(screenWidth, screenHeight, "LetBuildAbot");
	SetTargetFPS(60);

	Player player("assets/arrow.png");
	State state = {
		player,
	};
	MoveInstruction move;
	MoveInstruction move2;
	DelayInstruction delay(1.5f);
	move.SetNext(&delay);
	delay.SetNext(&move2);
	move2.SetNext(nullptr);
	Camera2D camera = {};
	camera.offset = {screenWidth * 0.5f, screenHeight * 0.5f};
	camera.target = player.position;
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	Instruction* i = &move;
	i->Setup(state);
	while (!WindowShouldClose())
	{
		camera.target = player.position;
		BeginDrawing();
		ClearBackground(BLACK);
		if(i != nullptr)
		{
			if(!i->isDone(state))
			{
				i->Execute(state);
			}
			else {
				i = i->Next();
				if(i != nullptr)
					i->Setup(state);
			}
		}
		BeginMode2D(camera);
		DrawInfiniteGrid(camera, screenWidth, screenHeight);
		player.Draw();
		EndMode2D();

		if(i == nullptr) {
			DrawText("Done", 0, 0, 24, RAYWHITE);
		}
		else {
			MoveInstruction* _i = reinterpret_cast<MoveInstruction*>(i);
			DrawText(
			std::format("Current Position: {} \n Target Position: {} \n Distance: \n {}",player.position.x,_i->m_end.x,Vector2Distance(_i->m_end, player.position)).c_str(), 
			0, 
			0, 
			24, RAYWHITE);
		}
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
