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

    Player pl(50, 50);


    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> map{};
    map = MapGeneration();

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        pl.drawMap(window, map);
        pl.update(window, map);
        pl.draw(window);

        
        window.display();
    }

    return 0;


}
