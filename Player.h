#pragma once
#include <windows.h>
#include <vector>
#include <SFML\Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include "Map.h"
#include <array>
#include "GlobalSizes.h"

class Player 
{
    float x;
    float y;
    float direction = 0;
    float direction_vertical = 0;

    sf::Sprite player_sprite;
    sf::Texture player_texture;

    float size;

    float ray_x = x + 0.5f * CELL_SIZE; // Начальные координаты луча
    float ray_y = y + 0.5f * CELL_SIZE;

    //спрайты для 2д карты 
    sf::Sprite s_icons;
    sf::Texture t_icons;

    //std::array<sf::CircleShape, RAYS_AMOUNT> temp_c;
   // std::array<std::array<float, 3>, RAYS_AMOUNT> rays_position;
    

public:
    std::array<std::array<float, 3>, RAYS_AMOUNT> rays_out;
    std::array<sf::CircleShape, RAYS_AMOUNT> temp_c;
    Player(float _x, float _y);
    void setPosition(float _x, float _y);
    void update(const sf::RenderWindow& i_window, const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map);

    

    void draw(sf::RenderWindow& i_window);
    
    void ray_tracing(const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map); 

    void drawMap(sf::RenderWindow& i_window, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map);
    
    float getPositionX();
    float getPositionY();
    float getDirection();
};
