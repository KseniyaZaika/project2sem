#include "Player.h"
#include "GlobalSizes.h"
#include "Tools.h"
#include "Map.h"
#include <array>


Player::Player(float _x, float _y)
{
    x = _x; y = _y;
    player_texture.loadFromFile("textures/MapPlayer16.png");
    player_sprite.setTexture(player_texture);
    size = player_texture.getSize().y;

    //////������� ��� 2� �����
    ////sf::Texture t_icons;
    ////t_icons.loadFromFile("textures/MapWall32.png");
    ////sf::Sprite s_icons(t_icons);
    ////s_icons.setTextureRect(sf::IntRect(0, 0, CELL_SIZE, CELL_SIZE));

}

void Player::set_position(float _x, float _y)
{
    x = _x;
    y = _y;
}


void Player::update(const sf::RenderWindow& i_window, const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map)
{
    float move_x = 0;
    float move_y = 0;
    float acceleration = 1.0f;
    // Right-Left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
    {
        direction += ROTATION_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
    {
        direction -= ROTATION_SPEED;
    }

    //WASD
    direction = GetDegrees(direction);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        move_x += MOVEMENT_SPEED * cos(DegToRad(GetDegrees(90 + direction)));
        move_y -= MOVEMENT_SPEED * sin(DegToRad(GetDegrees(90 + direction)));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        move_x += MOVEMENT_SPEED * cos(DegToRad(GetDegrees(direction - 90)));
        move_y -= MOVEMENT_SPEED * sin(DegToRad(GetDegrees(direction - 90)));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        move_x -= MOVEMENT_SPEED * cos(DegToRad(GetDegrees(direction)));
        move_y += MOVEMENT_SPEED * sin(DegToRad(GetDegrees(direction)));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        move_x += MOVEMENT_SPEED * cos(DegToRad(GetDegrees(direction)));
        move_y -= MOVEMENT_SPEED * sin(DegToRad(GetDegrees(direction)));
    }

    direction = fmod(360 + fmod(direction, 360), 360);//fmod-������� �� ������� � ������� ����� � ��������� ������ ( ������� ���������� ��������� ����)

    // ���������, ���� �� ������������ � ������ ��� ������� ��������� � ����������
    if (wallCollision(x + move_x * acceleration, y + move_y * acceleration, i_map) == 0)
    {
        // ���� ��� ������������, ����������� �������� �������� � ����������
        move_x *= acceleration;
        move_y *= acceleration;
    }

    if (wallCollision(x + move_x, y + move_y, i_map) == 0)
    {
        x += move_x;
        y += move_y;
    }
    else if (wallCollision(x, y + move_y, i_map) == 0)
    {
        y += move_y;
    }
    else if (wallCollision(x + move_x, y, i_map) == 0)
    {
        x += move_x;
    }
    
}


void Player::drawMap(sf::RenderWindow& i_window, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map)  
{
    sf::Texture t_icons;
    t_icons.loadFromFile("textures/MapWall32.png");
    sf::Sprite s_icons(t_icons);
    s_icons.setTextureRect(sf::IntRect(0, 0, CELL_SIZE, CELL_SIZE));

    for (unsigned short i = 0; i < MAP_WIDTH; i++)
    {
        for (unsigned short j = 0; j < MAP_HEIGHT; j++)
        {
            if (map[i][j] == Cell::Wall)
            {
                s_icons.setPosition(i * CELL_SIZE, j * CELL_SIZE);
                i_window.draw(s_icons);
            }
        }
    }
}

void Player::draw(sf::RenderWindow& i_window)
{
    float frame_angle = 360.f / (player_texture.getSize().x / size);
    float shifted_direction = fmod(360 + fmod(direction + 0.5f * frame_angle, 360), 360);

    player_sprite.setPosition(x, y);

    player_sprite.setTextureRect(sf::IntRect(size * floor(shifted_direction / frame_angle), 0, size, size));
    
    i_window.draw(player_sprite);
}

//void Player::raycasting(const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, float player_x, float player_y, float player_direction, float fov, int ray_count)
//{
//    float ray_angle = player_direction - fov / 2; // ���� ������� ����
//
//    float angle_increment = fov / ray_count; // ��������� ���� ����� ������
//
//    for (int ray_index = 0; ray_index < ray_count; ray_index++)
//    {
//        // ��������� ���� ���� �� �������� � �������
//        float angle_rad = DegToRad(ray_angle);
//
//        float ray_x = player_x; // ��������� ���������� ����
//        float ray_y = player_y;
//
//        // ���, � ������� ��� ��������� �� ��� X � Y
//        float step_x = cos(angle_rad);
//        float step_y = sin(angle_rad);
//
//        bool hit_wall = false; // ����, ����������� �� ������������ � ������������
//
//        // ���� ��� �� ���������� � ������������ ��� �� ������ �� ������� �����
//        while (!hit_wall && ray_x >= 0 && ray_y >= 0 && ray_x < MAP_WIDTH && ray_y < MAP_HEIGHT)
//        {
//            // ���������� ��� �� ���
//            ray_x += step_x;
//            ray_y += step_y;
//
//            // ���������, ���� �� ������������ � ������������
//            if (i_map[static_cast<int>(ray_x)][static_cast<int>(ray_y)] == Cell::Wall)
//            {
//                hit_wall = true;
//            }
//        }
//
//        // ���������� ���������� ����, ��������, ������ ����� �� ������
//        if (hit_wall)
//        {
//            // ���������� �� ������ �� ����� ������������
//            float distance = sqrt((ray_x - player_x) * (ray_x - player_x) + (ray_y - player_y) * (ray_y - player_y));
//
//            // ������ ����� �� ������, ��������� ���������� ����������
//            drawWall(ray_index, distance);
//        }
//
//        // ����������� ���� ��� ���������� ����
//        ray_angle += angle_increment;
//    }
//}
