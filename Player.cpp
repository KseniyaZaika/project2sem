#include "Player.h"
#include "GlobalSizes.h"
#include "Tools.h"
#include "Map.h"
#include <array>
#include <iostream>

Player::Player(float _x, float _y)
{
    x = _x; y = _y;
    player_texture.loadFromFile("D:/repos/proj2/from/textures/MapPlayer16.png");
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

    direction = fmod(360 + fmod(direction, 360), 360);//fmod-остаток от делени€ в формате числа с плавающей точкой ( функци€ приведени€ диапазона угла)

    // ѕровер€ем, есть ли столкновение с стеной при попытке двигатьс€ с ускорением
    if (wallCollision(x + move_x * acceleration, y + move_y * acceleration, i_map) == 0)
    {
        // ≈сли нет столкновени€, увеличиваем скорость движени€ с ускорением
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
    //// ѕереводим угол луча из градусов в радианы
    //float angle_rad = DegToRad(direction);

    //ray_x = x + 0.5f * CELL_SIZE; // Ќачальные координаты луча
    //ray_y = y + 0.5f * CELL_SIZE;

    //// Ўаг, с которым луч двигаетс€ по оси X и Y
    //float step_x = cos(angle_rad);
    //float step_y = -sin(angle_rad);
    //int steps = 0;
    //bool hit_wall = false; // ‘лаг, указывающий на столкновение с преп€тствием

    //// ѕока луч не столкнетс€ с преп€тствием или не выйдет за пределы карты
    //while (!hit_wall && steps <= RENDER_DISTANCE)
    //{
    //    // ѕеремещаем луч на шаг
    //    ray_x += step_x;
    //    ray_y += step_y;
    //   

    //    // ѕровер€ем, есть ли столкновение с преп€тствием
    //    if (i_map[static_cast<int>(ray_x/CELL_SIZE)][static_cast<int>(ray_y/CELL_SIZE)] == Cell::Wall)
    //    {
    //        hit_wall = true;
    //    }
    //    steps += 1;
    //    
    //}
   // std::cout << steps << std::endl;
    // степом можно ограничить в будущем дальность рендера( дл€ открытых карт чтоб он не уходил в бесконечный цикл)

    ///////////

    //bool wall_exists = 0;

    //char cell_step_x = 0;
    //char cell_step_y = 0;

    //float ray_dirrection_x = cos(DegToRad(direction));
    //float ray_dirrection_y = -sin(DegToRad(direction));

    //float ray_length = 0;

    //float ray_center_x = x + 0.5f * CELL_SIZE;
    //float ray_center_x = x + 0.5f * CELL_SIZE;// смещение в центр спрайта


    
}

void Player::draw(sf::RenderWindow& i_window)
{
    float frame_angle = 360.f / (player_texture.getSize().x / size);
    float shifted_direction = fmod(360 + fmod(direction + 0.5f * frame_angle, 360), 360);

    player_sprite.setPosition(x, y);
    player_sprite.setTextureRect(sf::IntRect(size * floor(shifted_direction / frame_angle), 0, size, size));
    
    i_window.draw(player_sprite);

    for (int i = 0; i < RAYS_AMOUNT; i++)
    {
        temp_c[i].setPosition(rays_out[i][0] - RADIUS, rays_out[i][1] - RADIUS);
        i_window.draw(temp_c[i]);

       // std::cout << rays_position[i][0] << " " << rays_position[i][1]  << " " << rays_position[i][2] << std::endl;
    }
   // i_window.draw(temp_c[0]);

   
}

void Player::ray_tracing(const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map)
{
    
    // ѕереводим угол луча из градусов в радианы
   // std::array<std::array<float, 3>, RAYS_AMOUNT> rays_out;

    float angle_rad = DegToRad(direction)- PI/4;
    float angle_step = PI/(2 * RAYS_AMOUNT);

    for (int i = 0; i < RAYS_AMOUNT; i++)
    {
        ray_x = x + 0.5f * CELL_SIZE; // Ќачальные координаты луча
        ray_y = y + 0.5f * CELL_SIZE;

        // Ўаг, с которым луч двигаетс€ по оси X и Y
        float step_x = cos(angle_rad);
        float step_y = -sin(angle_rad);
        int steps = 0;
        bool hit_wall = false; // ‘лаг, указывающий на столкновение с преп€тствием

        // ѕока луч не столкнетс€ с преп€тствием или не выйдет за пределы карты
        while (!hit_wall && steps <= RENDER_DISTANCE)
        {
            // ѕеремещаем луч на шаг
            ray_x += step_x;
            ray_y += step_y;


            // ѕровер€ем, есть ли столкновение с преп€тствием
            if (i_map[static_cast<int>(ray_x / CELL_SIZE)][static_cast<int>(ray_y / CELL_SIZE)] == Cell::Wall)
            {
                hit_wall = true;
            }
            steps += 1;

        }
        float distance = sqrt((ray_x - x) * (ray_x - x) + (ray_y - y) * (ray_y - y));
        // это “ќ„Ќќ не теорема пифагора!!!!!!!!!!!!!
       
        rays_out[i][2] = distance; ///потом добавить четвертвую размерность дл€ высоты
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
