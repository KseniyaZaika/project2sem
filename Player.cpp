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
        //float ray_direction = GetDegrees(direction + FOV * (floor(0.5f * RAYS_AMOUNT) - 1 - i) / (RAYS_AMOUNT - 1));
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
    //закончил на этом моменте
    float floor_level = round(0.5f * SCREEN_HEIGHT * (1 + tan(DegToRad(direction_vertical)) / tan(DegToRad(1 + FOV_VERTICAL))));

    //пол
    sf::RectangleShape floor_shape(sf::Vector2f(SCREEN_WIDTH, 0.5f * SCREEN_HEIGHT));
    floor_shape.setFillColor(sf::Color(146, 146, 100));
    floor_shape.setPosition(0, 0.5f * SCREEN_HEIGHT);
    i_window.draw(floor_shape);

    for (unsigned short i = 0; i < SCREEN_WIDTH; i++)
    {
        //устранение фишай
       // float ray_direction = FOV * (floor(0.5f * SCREEN_WIDTH) - i) / (SCREEN_WIDTH - 1);
        //float ray_projection_posittion = 0.5f * tan(DegToRad(rays_out[i][0])) / tan(DegToRad(0.5f * FOV));

        //short current_column = static_cast<short>(round(SCREEN_WIDTH * (0.5f - ray_projection_posittion)));
        float shape_height = round(SCREEN_HEIGHT * projection_distance / rays_out[i][2]);

        sf::RectangleShape shape(sf::Vector2f(1, shape_height));
        shape.setFillColor(sf::Color(0, 255 * (1 - rays_out[i][2] / RENDER_DISTANCE), 0));
        shape.setPosition(i, 0.5f * (SCREEN_HEIGHT - shape_height));

        i_window.draw(shape);
    }
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
        // это “ќ„Ќќ не теорема пифагора!!!!!!!!!!!!! ашалеть, а что же это тогда
       
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
