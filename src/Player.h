#pragma once

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
public:
  Player(const std::string &texturePath);

  ~Player() { UnloadTexture(m_texture); }

  Player(const Player &) = delete;
  Player &operator=(const Player &) = delete;

  bool Update();
  void Draw() const;
  Vector2 GetFacingVector() const;

private:
	Texture2D m_texture = {};
};