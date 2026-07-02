
constexpr float s_tileSize = 64.0f;

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
