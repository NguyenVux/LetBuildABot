#include "../Player.h"
#include "../Runner.h"
#include "Instruction.h"
#include "raylib.h"
#include "raymath.h"

void MoveInstruction::Setup(State& state)
{
    Player& player = state.player;
    m_start = player.position;
    m_end = m_start + player.GetFacingVector() * s_tileSize;
}

void MoveInstruction::Execute(State& state)
{
    Player& player = state.player;
    float step = GetFrameTime() * s_tileSize;
    float remaining = Vector2Distance(player.position, m_end);

    if (step >= remaining)
        player.position = m_end;
    else
        player.position += player.GetFacingVector() * step;
}

bool MoveInstruction::isDone(State& state) const
{
    float dist = Vector2Distance(m_end, state.player.position);
    return dist <= EPSILON;
}

void DelayInstruction::Setup(State& state)
{
}

void DelayInstruction::Execute(State& state)
{
    m_elapsed += GetFrameTime();
}

bool DelayInstruction::isDone(State& state) const
{
    return m_elapsed >= m_duration;
}

void RotateInstruction::Setup(State& state)
{
    Player& player = state.player;
    m_startAngle = player.angle;
    m_targetAngle = Wrap(m_startAngle + m_angle, 0.0f, 360.0f);
}

void RotateInstruction::Execute(State& state)
{
    Player& player = state.player;

    float step = GetFrameTime() * m_angle;
    float delta = abs(player.angle - m_targetAngle);

    if (abs(step) >= delta)
    {
        player.angle = m_targetAngle;
    }
    else
    {
        player.angle += step;
        player.angle = Wrap(player.angle, 0.0f, 360.0f);
    }
}

bool RotateInstruction::isDone(State& state) const
{
    Player& player = state.player;
    float delta = abs(player.angle - m_targetAngle);
    return delta <= EPSILON;
}