#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Player.h"
#include "Game.h"
int main()
{
    float deltaTime;

    unsigned int SCREEN_WIDTH = 1920;
    unsigned int SCREEN_HEIGHT = 1080;
    //Open Window
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }), "AstronautGame");
    //window->setFramerateLimit(120);

    sf::Clock frameClock;


    Begin(window);

    // Main/Game Loop
    while (window->isOpen())
    {
        //std::cout << "Jump Clock:" << jumpClock.getElapsedTime().asSeconds() << " isJumping: " << isJumping << std::endl;
        // Event loop
        while (const std::optional event = window->pollEvent())
        {
            // User is closing window
            if (event->is<sf::Event::Closed>())
                window->close();
            if (event->is<sf::Event::KeyReleased>())
            {
                player->isJumping = false;
            }
        }

        deltaTime = frameClock.restart().asSeconds(); // Time between frames

        Update(deltaTime, window);

        Render(window);
    }

    delete player;
    delete window;
    return 0;
}