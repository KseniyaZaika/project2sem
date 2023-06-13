#pragma once
#include <windows.h>
#include <vector>
#include <SFML\Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

class Player // 
{
    float x;
    float y;
    float direction = 0;
    sf::Sprite player_sprite;
    sf::Texture player_texture;
    float size;

public:
    Player(float _x, float _y);
    void set_position(float _x, float _y);
    void update(const sf::RenderWindow& i_window);
    void draw(sf::RenderWindow& i_window);
    
};