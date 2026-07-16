#include "Player.h"



Player::Player(const std::string &texturePath) {
    m_texture = LoadTexture(texturePath.c_str());
}

void Player::Draw() const
{
    const Vector2 pixelSize = Vector2Scale(size, s_tileSize);
    const Rectangle source = {0.0f, 0.0f, static_cast<float>(m_texture.width), static_cast<float>(m_texture.height)};
    const Rectangle destination = {
        position.x,
        position.y,
        pixelSize.x,
        pixelSize.y,
    };
    DrawTexturePro(m_texture, source, destination, Vector2Scale(pixelSize, 0.5f), (int)facing * 90.0f, WHITE);
}


Vector2 Player::GetFacingVector() const 
{
    switch (facing) {
        case Facing::Right:
            return {
                1.0f,
                0.0f,
            };
            break;
        case Facing::Down:
            return {
                0.0f,
                -1.0f,
            };
            break;
        case Facing::Left:
            return {
                -1.0f,
                0.0f,
            };
            break;
        case Facing::Up:
            return {
                0.0f,
                1.0f,
            };
            break;
        case Facing::Count:
            break;
    }
    throw std::string("Uknow Facing");
}