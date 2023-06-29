#pragma once

#include <windows.h>
#include "GlobalSizes.h"
#include <SFML\Graphics.hpp>
#include <SFML/Window.hpp>
//#include <math.h>
//#include <vector>

sf::Vector2f GetCursorPosition(sf::RenderWindow& window);

bool IsInsideRect(sf::Vector2f cursorPosition, sf::RectangleShape* rectangle);

float DistanceBetweenPoints(sf::Vector2f p1, sf::Vector2f p2);

float RadToDeg(float radians);

float DegToRad(float deg);

float GetDegrees(float i_degrees);

template <typename type>
void Swap(type* a, type* b);

template <typename type>//????????????????????
bool BelongingInterval(type min, type max, type num, float error = 0);



//sf::Color CorrectLightness(sf::Color color, int delta);


/// 
/// 



sf::Vector2f GetCursorPosition(sf::RenderWindow& window)
{
    sf::Vector2f cursorPosition;
    cursorPosition = (sf::Vector2f)sf::Mouse::getPosition(window);
    cursorPosition = window.mapPixelToCoords((sf::Vector2i)cursorPosition);
    return cursorPosition;
}


bool IsInsideRect(sf::Vector2f cursorPosition, sf::RectangleShape* rectangle)
{
    sf::Vector2f position = rectangle->getPosition();
    sf::Vector2f size = rectangle->getSize();
    sf::Vector2f scale = rectangle->getScale();
    size = { size.x * scale.x, size.y * scale.y };

    if (cursorPosition.x >= position.x && cursorPosition.x <= position.x + size.x &&
        cursorPosition.y >= position.y && cursorPosition.y <= position.y + size.y)
    {
        return 1;
    }
    return 0;
}



float DistanceBetweenPoints(sf::Vector2f p1, sf::Vector2f p2)
{
    float distance;
    distance = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
    return distance;
}

float RadToDeg(float radians)
{
    return 180 * radians / PI;
}

float DegToRad(float deg)
{
    return deg * PI / 180;
}

float GetDegrees(float degrees)
{
    return static_cast<float>(fmod(360 + fmod(degrees, 360), 360));
}


template <typename type>
void Swap(type* a, type* b)
{
    type c = (*a);
    (*a) = (*b);
    (*b) = c;
}

template <typename type>
bool BelongingInterval(type min, type max, type num, float error)
{
    if (min > max) Swap(&min, &max);

    if (num >= min - error && num <= max + error) return true;
    else return false;
}


//sf::Color CorrectLightness(sf::Color color, int delta)
//{
//    int r = std::min(std::max(color.r + delta * 30 / 100, 0), 255);
//    int g = std::min(std::max(color.g + delta * 59 / 100, 0), 255);
//    int b = std::min(std::max(color.b + delta * 11 / 100, 0), 255);
//
//    return sf::Color(r, g, b);
//}