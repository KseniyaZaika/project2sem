#include "Map.h"
#include <iostream>


std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> MapGeneration() // Steven& i_steven//Player& i_player
{
    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> out_map{};

    sf::Image map_sketch;
    map_sketch.loadFromFile("D:/repos/proj2/from/textures/MapSketch.png");

    for (unsigned char y = 0; y < MAP_WIDTH; y++)
    {
        for (unsigned char x = 0; x < MAP_HEIGHT; x++)
        {
            sf::Color pixel = map_sketch.getPixel(y, x);

            if (pixel == sf::Color(0, 0, 0))
            {
                out_map[y][x] = Cell::Wall;
            }
            /* else if (pixel == sf::Color(255, 0, 0))
             {
                 i_player.set_position(static_cast<float>(CELL_SIZE * y), static_cast<float>(CELL_SIZE * x));
             }*/
             /*else if (pixel == sf::Color(0, 0, 255))
             {
                 i_steven.set_position(static_cast<float>(CELL_SIZE * y), static_cast<float>(CELL_SIZE * x));
             }*/
            else
            {
                out_map[y][x] = Cell::Empty;
            }
        }
    }

    return out_map;
}

bool wallCollision(float x, float y, const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map)
{
    // Получение позиции ячеек, в которых находятся углы хитбокса персонажа
    int cellLeft = static_cast<int>(x / CELL_SIZE);
    int cellTop = static_cast<int>(y / CELL_SIZE);
    int cellRight = static_cast<int>((x + 16) / CELL_SIZE);
    int cellBottom = static_cast<int>((y + 16) / CELL_SIZE);

    // Проверка нахождения углов хитбокса внутри карты
    if (cellLeft < 0 || cellRight >= MAP_WIDTH || cellTop < 0 || cellBottom >= MAP_HEIGHT)
    {
        return true;
    }

    // Проверка столкновения с элементами карты
    for (int i = cellLeft; i <= cellRight; i++)
    {
        for (int j = cellTop; j <= cellBottom; j++)
        {
            if (i_map[i][j] == Cell::Wall)
            {
                return true;
            }
        }
    }
    return false;
}