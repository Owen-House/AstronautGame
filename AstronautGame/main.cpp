#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "Resources.h"

int main()
{
    float deltaTime;

    unsigned int SCREEN_WIDTH = 1920;
    unsigned int SCREEN_HEIGHT = 1080;
    sf::Clock frameClock;
    
    //Open Window
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }), "AstronautGame");
    //window->setFramerateLimit(240);

    Renderer renderer(*window);

    Begin(window);

    // Main/Game Loop
    while (window->isOpen())
    {
        deltaTime = frameClock.restart().asSeconds(); // Time between frames


        player->setVelocity({ 0.0, 0.0 });

        // Event loop
        while (const std::optional event = window->pollEvent())
        {
            // User is closing window
            if (event->is<sf::Event::Closed>())
                window->close();
            if (event->is<sf::Event::KeyReleased>())
            {
                auto keyEvent = event->getIf<sf::Event::KeyReleased>();
                player->isJumping = false;
                //endOfMovement(keyEvent, deltaTime);
            }
        }


        Update(deltaTime, window);


        Render(window, renderer);


    }

    delete player;
    delete window;
    return 0;
}