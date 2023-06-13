#include "Player.h"
#include "GlobalSizes.h"
#include "Tools.h"

Player::Player(float _x, float _y)
{
    x = _x; y = _y;
    player_texture.loadFromFile("D:/repos/proj2/from/textures/MapPlayer16.png");
    player_sprite.setTexture(player_texture);
    size = player_texture.getSize().y;
}

void Player::set_position(float _x, float _y)
{
    x = _x;
    y = _y;
}

void Player::update(const sf::RenderWindow& i_window)
{
    // Right-Left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        direction += ROTATION_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        direction -= ROTATION_SPEED;
    }

    //WASD
    direction = GetDegrees(direction);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        x += MOVEMENT_SPEED * cos(DegToRad(GetDegrees(90 + direction)));
        y -= MOVEMENT_SPEED * sin(DegToRad(GetDegrees(90 + direction)));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        x += MOVEMENT_SPEED * cos(DegToRad(GetDegrees(direction - 90)));
        y -= MOVEMENT_SPEED * sin(DegToRad(GetDegrees(direction - 90)));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        x -= MOVEMENT_SPEED * cos(DegToRad(GetDegrees(direction)));
        y += MOVEMENT_SPEED * sin(DegToRad(GetDegrees(direction)));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        x += MOVEMENT_SPEED * cos(DegToRad(GetDegrees(direction)));
        y -= MOVEMENT_SPEED * sin(DegToRad(GetDegrees(direction)));
    }

    direction = fmod(360 + fmod(direction, 360), 360);//fmod-остаток от деления в формате числа с плавающей точкой ( функция приведения диапазона угла)
}

void Player::draw(sf::RenderWindow& i_window)
{
    float frame_angle = 360.f / (player_texture.getSize().x / size);
    float shifted_direction = fmod(360 + fmod(direction + 0.5f * frame_angle, 360), 360);

    player_sprite.setPosition(x, y);

    player_sprite.setTextureRect(sf::IntRect(size * floor(shifted_direction / frame_angle), 0, size, size));
    //player_sprite.setTextureRect(sf::IntRect(0,0,32,32));
    i_window.draw(player_sprite);
}


