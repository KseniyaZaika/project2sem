#pragma once
#include <windows.h>
#include <vector>
#include <SFML\Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include "Map.h"
#include <array>
#include "GlobalSizes.h"

class Player // 
{
    float x;
    float y;
    float direction = 0;
    sf::Sprite player_sprite;
    sf::Texture player_texture;
    float size;

    //спрайты для 2д карты возможно потом убрать надо будет
    sf::Sprite s_icons;
    sf::Texture t_icons;

public:
    Player(float _x, float _y);
    void set_position(float _x, float _y);
    void update(const sf::RenderWindow& i_window, const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map);

    //отрисовка 2д карты
    void drawMap(sf::RenderWindow& i_window, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map);

    void draw(sf::RenderWindow& i_window);
    
};