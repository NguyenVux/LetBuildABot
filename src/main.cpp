#include "Instruction/Instruction.h"
#include "Player.h"
#include "Runner.h"
#include "raylib.h"
#include "raymath.h"
#include <format>
#include <vector>

void DrawInfiniteGrid(const Camera2D& camera, int screenWidth, int screenHeight);

class Block
{
public:
	Rectangle rect;
	void Update() const {
		Vector2 mousePos = GetMousePosition();
		bool hover = CheckCollisionPointRec(mousePos, rect);
		Color color = GREEN;
		if(hover)
		{
			color = RED;
		}
		DrawRectangleRec(rect,color);

	}
};

struct Thickness {
	float top;
	float bottom;
	float left;
	float right;
};
class WidgetContainer {
public:
	Thickness paddings;
	Thickness margins;
	std::vector<Block> m_buttons;
	Rectangle rect;

	void load()
	{
		for(int i = 0; i < m_buttons.size(); i++)
		{
			Block& block = m_buttons[i];	
			if(i == 0)
			{
				block.rect.x = rect.x + paddings.left;
				block.rect.y = rect.y + paddings.top;
			}
			else
			{
				Block& prevBlock = m_buttons[i-1];
				block.rect.x = rect.x;
				block.rect.y = prevBlock.rect.y + prevBlock.rect.height; 
			}
		}
	}

	void Draw()
	{
		for(const Block& block : m_buttons)
		{
			block.Update();
		}
	}
	
};

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

    Instruction* i = &move;
    i->Setup(state);
    // i = nullptr;
    WidgetContainer cont;
    cont.rect.x = 0;
    cont.rect.y = 0;
    for(int i = 0; i < 10; i++)
    {
	    Block block;
	    block.rect.width = 80;
	    block.rect.height = 30;
	    cont.m_buttons.push_back(block);

    }
    cont.rect.x = screenWidth - 80;
    cont.load();

    while (!WindowShouldClose())
    {
        camera.target = player.position;
        BeginDrawing();
        ClearBackground(BLACK);
        if (i != nullptr)
        {
            if (!i->isDone(state))
            {
                i->Execute(state);
            }
            else
            {
                i = i->Next();
                if (i != nullptr)
                    i->Setup(state);
            }
        }

	cont.Draw();
        BeginMode2D(camera);
        DrawInfiniteGrid(camera, screenWidth, screenHeight);
        player.Draw();
        EndMode2D();

        if (i == nullptr)
        {
            DrawText("Done", 0, 0, 24, RAYWHITE);
        }
        else
        {
            MoveInstruction* _i = reinterpret_cast<MoveInstruction*>(i);
            DrawText(std::format("Current Position: {} \n"
                                 "Target Position: {} \n"
                                 "Distance: \n {}",
                                 player.position.x, _i->m_end.x,
                                 Vector2Distance(_i->m_end, player.position))
                         .c_str(),
                     0, 0, 24, RAYWHITE);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
