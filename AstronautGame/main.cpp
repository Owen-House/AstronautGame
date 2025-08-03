#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "Resources.h"
#include "Camera.h"

int main()
{
    float deltaTime;

    unsigned int SCREEN_WIDTH = 1920;
    unsigned int SCREEN_HEIGHT = 1080;
    sf::Clock frameClock;
    sf::Clock frameRate;
    int FPS = 0;
    
    //Open Window
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }), "AstronautGame");
    Renderer renderer(*window);
    //window->setFramerateLimit(240);

    
    // Inital Game Setup
    Begin(window);

    // Main/Game Loop
    while (window->isOpen())
    {
        FPS++;
        deltaTime = frameClock.restart().asSeconds(); // Time between frames


        player->setVelocity({ 0.0, 0.0 });

        // Event loop
        while (const std::optional event = window->pollEvent())
        {
            // User is closing window
            if (event->is<sf::Event::Closed>())
                window->close();
            //User Released Key
            if (event->is<sf::Event::KeyReleased>())
            {
                auto keyEvent = event->getIf<sf::Event::KeyReleased>();
                player->isJumping = false;
                //endOfMovement(keyEvent, deltaTime);
            }
        }

        window->setView(camera.GetView(window->getSize()));
        
        // Update Game Logic
        Update(deltaTime, window);

        if (frameRate.getElapsedTime().asSeconds() >= 1.0)
        {
            frameRate.restart();
            std::cout << "FPS: " << FPS << std::endl;
            FPS = 0;
        }

        // Display to Window
        window->clear();
        Render(window, renderer);
        window->display();
    }

    delete player;
    delete window;
    return 0;
}