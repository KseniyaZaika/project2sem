#include <windows.h>
#include <vector>
#include <SFML\Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#
//#include "Tools.h"
#include "GlobalSizes.h"
#include "Player.h"

//std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> MapGeneration(Player& i_player);
//
//std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> MapGeneration(Player& i_player) //convert_sketch(Player& i_player)//, Steven& i_steven
//{
//    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> output_map{};
//
//    sf::Image map_sketch;
//    map_sketch.loadFromFile("Resources/Images/MapSketch.png");
//
//    for (unsigned char a = 0; a < MAP_WIDTH; a++)
//    {
//        for (unsigned char b = 0; b < MAP_HEIGHT; b++)
//        {
//            sf::Color pixel = map_sketch.getPixel(a, b);
//
//            if (pixel == sf::Color(0, 0, 0))
//            {
//                output_map[a][b] = Cell::Wall;
//            }
//            else if (pixel == sf::Color(255, 0, 0))
//            {
//                i_player.set_position(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));
//            }
//            /*else if (pixel == sf::Color(0, 0, 255))
//            {
//                i_steven.set_position(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));
//            }*/
//            else
//            {
//                output_map[a][b] = Cell::Empty;
//            }
//        }
//    }
//
//    return output_map;
//}



int main() {

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "window", sf::Style::Close);
    window.setVerticalSyncEnabled(true);
   // window.setMouseCursorVisible(0);
    window.setView(sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));


    Player pl(50, 50);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        pl.update(window);
        pl.draw(window);

        window.display();
    }

    return 0;


}
