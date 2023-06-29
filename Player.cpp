#include "Player.h"
#include "GlobalSizes.h"
#include "Tools.h"
#include "Map.h"
#include <array>
#include <iostream>

Player::Player(float _x, float _y)
{
    x = _x; y = _y;
    player_texture.loadFromFile("textures/MapPlayer16.png");
    player_sprite.setTexture(player_texture);
    size = player_texture.getSize().y;

    for (int i = 0; i < RAYS_AMOUNT; i++)
    {
        temp_c[i].setRadius(RADIUS);
        temp_c[i].setFillColor(sf::Color(150, 0, 0));
    }

}

void Player::setPosition(float _x, float _y)
{
    x = _x;
    y = _y;
}


void Player::update(const sf::RenderWindow& i_window, const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map)
{
    float move_x = 0;
    float move_y = 0;
    float acceleration = 1.5f;
    float rotation_x = 0;//horizontal
    float rotation_y = 0;//vertical
    
    
    // Right-Left
   /* if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
    {
        direction += ROTATION_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
    {
        direction -= ROTATION_SPEED;
    }*/

  
    float window_center_x = 0.5f * i_window.getSize().x;
    float window_center_y = 0.5f * i_window.getSize().y;
    rotation_x = FOV * (window_center_x - sf::Mouse::getPosition(i_window).x) / i_window.getSize().x;
    rotation_y = FOV_VERTICAL * (window_center_y - sf::Mouse::getPosition(i_window).y) / i_window.getSize().y;
    direction = GetDegrees(direction - rotation_x);
    direction_vertical = std::clamp<float>(direction_vertical + rotation_y, -89, 89);// мб на + посенять
    
    sf::Mouse::setPosition(sf::Vector2i(window_center_x, window_center_y), i_window);

    //WASD
    direction = GetDegrees(direction);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        move_x += MOVEMENT_SPEED * cos(DegToRad(GetDegrees(90 + direction)));
        move_y -= MOVEMENT_SPEED * sin(DegToRad(GetDegrees(90 + direction)));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
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

    direction = fmod(360 + fmod(direction, 360), 360);//fmod-остаток от деления в формате числа с плавающей точкой ( функция приведения диапазона угла)
    // Проверяем, есть ли столкновение с стеной при попытке двигаться с ускорением
     
    if (wallCollision(x + move_x * acceleration, y + move_y * acceleration, i_map) == 0)
    {
        // Если нет столкновения, увеличиваем скорость движения с ускорением
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

    ray_tracing(i_map);
   
}

void Player::drawMap(sf::RenderWindow& i_window, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map)
{
    float frame_angle = 360.f / (player_texture.getSize().x / size);
    float shifted_direction = fmod(360 + fmod(direction + 0.5f * frame_angle, 360), 360);

    player_sprite.setPosition(x, y);
    player_sprite.setTextureRect(sf::IntRect(size * floor(shifted_direction / frame_angle), 0, size, size));
    i_window.draw(player_sprite);

    sf::VertexArray fov_visualisation(sf::TriangleFan, 1 + RAYS_AMOUNT);
    fov_visualisation[0].position = sf::Vector2f(x + size * 0.5f, y + size * 0.5f);

    for (int i = 0; i < RAYS_AMOUNT; i++)
    {
        fov_visualisation[1 + i].position = sf::Vector2f(rays_out[i][0], rays_out[i][1]);
    }
    i_window.draw(fov_visualisation);

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
    float projection_distance = 0.5f * CELL_SIZE / tan(DegToRad(0.5f * FOV_VERTICAL));

    //ïîë
    float floor_level = round(0.5f * SCREEN_HEIGHT * (1 + tan(DegToRad(direction_vertical)) / tan(DegToRad(1 + FOV_VERTICAL))));
    sf::RectangleShape floor_shape(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT - floor_level));
    floor_shape.setFillColor(sf::Color(146, 146, 100));
    floor_shape.setPosition(0, floor_level);
    i_window.draw(floor_shape);

    for (unsigned short i = 0; i < SCREEN_WIDTH; i++)
    {
        //óñòðàíåíèå ôèøàé
        float ray_direction = FOV * (floor(0.5f * SCREEN_WIDTH) - i) / (SCREEN_WIDTH - 1);
        float ray_projection_posittion = 0.5f * tan(DegToRad(ray_direction)) / tan(DegToRad(0.5f * FOV));
        short current_column = static_cast<short>(round(SCREEN_WIDTH * (0.5f - ray_projection_posittion)));
        //
        /*if (i < SCREEN_WIDTH - 1)
        {

        }*/
        float shape_height = round(SCREEN_HEIGHT * projection_distance / (rays_out[i][2]*cos(DegToRad(ray_direction))));
       
        sf::RectangleShape shape(sf::Vector2f(1, shape_height));
        shape.setFillColor(sf::Color(0, 255 * (1 - rays_out[i][2] / RENDER_DISTANCE), 0));
        //shape.setPosition(i, 0.5f * (SCREEN_HEIGHT - shape_height));
        shape.setPosition(i, round(floor_level - 0.5f * shape_height));
        i_window.draw(shape);
    }
}

void Player::ray_tracing(const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map)
{

   
    float angle_rad = DegToRad(direction) - PI / 4;
    float angle_step = PI / (2 * RAYS_AMOUNT);

    for (int i = 0; i < RAYS_AMOUNT; i++)
    {
        ray_x = x + 0.5f * CELL_SIZE; // начальные коорд
        ray_y = y + 0.5f * CELL_SIZE;

        // Шаг, с которым луч двигается по оси X и Y
        float step_x = cos(angle_rad);
        float step_y = -sin(angle_rad);
        int steps = 0;
        bool hit_wall = false; 
       
        while (!hit_wall && steps <= RENDER_DISTANCE)
        {          
            ray_x += step_x;
            ray_y += step_y;

            if (i_map[static_cast<int>(ray_x / CELL_SIZE)][static_cast<int>(ray_y / CELL_SIZE)] == Cell::Wall)
            {
                hit_wall = true;
            }
            steps += 1;

        }
        float distance = sqrt((ray_x - x) * (ray_x - x) + (ray_y - y) * (ray_y - y));

        rays_out[i][2] = distance; ///потом добавить четвертвую размерность для высоты
        rays_out[i][0] = ray_x;
        rays_out[i][1] = ray_y;
        angle_rad += angle_step;
    }

}


float Player::getPositionX()
{
    return x;
}
float Player::getPositionY()
{
    return y;
}
float Player::getDirection()
{
    return direction;
}

//void Player::draw(sf::RenderWindow& i_window)
//{
//    float frame_angle = 360.f / (player_texture.getSize().x / size);
//    float shifted_direction = fmod(360 + fmod(direction + 0.5f * frame_angle, 360), 360);
//
//    player_sprite.setPosition(x, y);
//    player_sprite.setTextureRect(sf::IntRect(size * floor(shifted_direction / frame_angle), 0, size, size));
//
//    i_window.draw(player_sprite);
//
//    for (int i = 0; i < RAYS_AMOUNT; i++)
//    {
//        temp_c[i].setPosition(rays_out[i][0] - RADIUS, rays_out[i][1] - RADIUS);
//        i_window.draw(temp_c[i]);
//
//        // std::cout << rays_position[i][0] << " " << rays_position[i][1]  << " " << rays_position[i][2] << std::endl;
//    }
//    // i_window.draw(temp_c[0]);
//
//
//}