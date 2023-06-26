#include <windows.h>
#include <vector>
#include <SFML\Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <array>

//#include "Tools.h"
#include "GlobalSizes.h"
#include "Player.h"
#include "Map.h"


// кстати потом в конце надо будет пофиксить прямые пути спрайта
// это из симейк файла делается, но мне лень сейчас. 

int main() {

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "window", sf::Style::Close);
    window.setVerticalSyncEnabled(true);
   //window.setMouseCursorVisible(0);
    window.setView(sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));
    window.setFramerateLimit(60);

    Player pl(50, 50);


    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> map{};
    map = MapGeneration();

    sf::Texture t_icons;
    t_icons.loadFromFile("textures/MapWall32.png");
    sf::Sprite s_icons(t_icons);
    s_icons.setTextureRect(sf::IntRect(0, 0, CELL_SIZE, CELL_SIZE));


    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(73,255,255));
        
        ////for (int i = 0; i < RAYS_AMOUNT; i++)
        ////{
        ////    pl.temp_c[i].setPosition(pl.rays_position[i][0] - RADIUS, pl.rays_position[i][1] - RADIUS);
        ////    window.draw(pl.temp_c[i]);
        ////    //std::cout << i << std::endl;
        ////}
        ////i_window.draw(temp_c[0]);
        //window.draw(pl.temp_c[0]);


        pl.drawMap(window, map);
        pl.update(window, map);
        pl.draw(window);
        
        
        window.display();
    }

    return 0;


}
