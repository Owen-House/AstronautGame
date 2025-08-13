#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "Resources.h"
#include "Camera.h"
#include "Menu.h"

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

    //Main Menu
    Menu menu(camera, window);

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
            {
                window->close();
            }
            // Window is not focused
            if (event->is<sf::Event::FocusLost>() || event->is<sf::Event::Resized>())
            {
                inMenu = true;
            }
            // User Released Key
            if (event->is<sf::Event::KeyReleased>())
            {
                auto keyEvent = event->getIf<sf::Event::KeyReleased>();
                if (keyEvent->code == sf::Keyboard::Key::Up || keyEvent->code == sf::Keyboard::Key::W)
                {
                    menu.moveUp();
                }
                if (keyEvent->code == sf::Keyboard::Key::Down || keyEvent->code == sf::Keyboard::Key::S)
                {
                    menu.moveDown();
                }
                if (keyEvent->code == sf::Keyboard::Key::Enter || keyEvent->code == sf::Keyboard::Key::Space)
                {
                    switch (menu.getPressedItem())
                    {
                    case 0:
                        // Title
                        break;
                    case 1:
                        std::cout << "Play Button Pressed!" << std::endl;
                        inMenu = false;
                        break;
                    case 2:
                        std::cout << "Levels Button Pressed!" << std::endl;
                        break;
                    case 3:
                        std::cout << "Exit Button Pressed!" << std::endl;
                        window->close();
                        break;
                    }
                }
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                {
                    inMenu = true;
                }
                player->isJumping = false;
            }
        }

        window->setView(camera.GetView(window->getSize()));
        
        // Only start game when not in menu
        if (!inMenu)
        {
            // Update Game Logic
            Update(deltaTime, window);
            window->clear();
            Render(window, renderer);
        } 
        else 
        {
            menu.updatePosition(camera, window);
            menu.updateLevel(camera, window, currentLevel);
            window->clear();
            menu.draw(window, renderer);
        }
        
        // FPS counter
        if (frameRate.getElapsedTime().asSeconds() >= 1.0)
        {
            frameRate.restart();
            std::cout << "FPS: " << FPS << std::endl;
            FPS = 0;
        }

        
        window->display();
    }

    delete player;
    delete window;
    return 0;
}