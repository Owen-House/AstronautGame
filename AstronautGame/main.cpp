#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Player.h"

int main()
{

    
    unsigned int SCREEN_WIDTH= 1920;
    unsigned int SCREEN_HEIGHT = 1080;

    float astronautScale = 6.f;
    float numGroundSurfaces = 2.0;
    sf::Clock clock;

    const float gridSize = 150.f;
    sf::Vector2f mousePosGrid;

    sf::FloatRect nextPos;

    // Gravity
    float playerSpeed = 800.0;
    const float groundHeight = float(SCREEN_HEIGHT - 300);
    const float gravitySpeed = 0.3;
    bool isJumping = false;


    //Open Window
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }), "AstronautGame");
    //window->setFramerateLimit(120);


#pragma region Textures

    sf::Texture astronautTexture;
    if (!astronautTexture.loadFromFile("resources/AstronautAnimations.png"))
    {
        std::cerr << "ERROR::COULD NOT LOAD FILE::resources/AstronautAnimations.png" << std::endl;
        return -1;
    }

    sf::Texture moonTexture;
    if (!moonTexture.loadFromFile("resources/MoonSurface.png"))
    {
        std::cerr << "ERROR::COULD NOT LOAD FILE::resources/MoonSurface.png" << std::endl;
        return -1;
    }

#pragma endregion


    // Load Player
    sf::Sprite astronautSprite(astronautTexture);
    Player player({ 9, 10 }, { 80, float(SCREEN_HEIGHT - 300) }, sf::IntRect({ 0, 0 }, { 16, 16 }), astronautSprite, astronautScale);
    player.alignHitBoxToPlayer(3, 6);

    // Load Ground
    std::vector<sf::Sprite> moonSurfaces;
    for (auto i = 0; i < numGroundSurfaces; i++) 
    {
        sf::Sprite moonSprite(moonTexture);
        moonSprite.setOrigin({ 0, 32 });
        moonSprite.setPosition({ float(i * (SCREEN_WIDTH / numGroundSurfaces)), float(SCREEN_HEIGHT)});
        moonSprite.setScale(sf::Vector2f({ 7.5, 7.5 }));
        moonSurfaces.push_back(moonSprite);
    }
    
    // Platforms
    std::vector<sf::RectangleShape> platforms;
    sf::RectangleShape platform;

    platform.setFillColor(sf::Color::Red);
    platform.setSize(sf::Vector2f(gridSize, gridSize));
    platform.setPosition(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));

    // Main/Game Loop
    while (window->isOpen())
    {
        // Event loop
        while (const std::optional event = window->pollEvent())
        {
            // User is closing window
            if (event->is<sf::Event::Closed>())
                window->close();
        }

        float deltaTime = clock.restart().asSeconds(); // Time between frames

#pragma region Preparing Movement 

        
        sf::Vector2f velocity = { 0.0f, 0.0f };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
        {
            velocity.y = -playerSpeed * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
        {
            velocity.y = playerSpeed * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
        {
            velocity.x = playerSpeed * deltaTime;

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
        {
            velocity.x = -playerSpeed * deltaTime;

        }
#pragma endregion

#pragma region Platform Collision

        mousePosGrid.x = sf::Mouse::getPosition(*window).x / (int)gridSize;
        mousePosGrid.y = sf::Mouse::getPosition(*window).y / (int)gridSize;


        // Add Platforms
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            bool exists = false;
            for (size_t i = 0; i < platforms.size() && !exists; i++)
            {
                if (platforms[i].getPosition().x / (int)gridSize == mousePosGrid.x
                    && platforms[i].getPosition().y / (int)gridSize == mousePosGrid.y)
                {
                    exists = true;
                }
            }

            if (!exists)
            {
                platform.setPosition({ mousePosGrid.x * gridSize, mousePosGrid.y * gridSize});
                platforms.push_back(platform);
            }
        }

        for (auto& platform : platforms)
        {
            sf::FloatRect playerBounds = player.getHitbox().getGlobalBounds();
            sf::FloatRect platformBounds = platform.getGlobalBounds();

            nextPos = playerBounds;
            nextPos.position.x += velocity.x;
            nextPos.position.y += velocity.y;

            
            
            if (platformBounds.findIntersection(nextPos))
            {

                // Platform Top Collision
                if (playerBounds.position.y < platformBounds.position.y
                    && playerBounds.position.y + playerBounds.size.y < platformBounds.position.y + platformBounds.size.y
                    && playerBounds.position.x < platformBounds.position.x + platformBounds.size.x
                    && playerBounds.position.x + playerBounds.size.x > platformBounds.position.x)
                {
                    velocity.y = 0.f;
                    player.getHitbox().setPosition({playerBounds.position.x, platformBounds.position.y - playerBounds.size.y});
                }

                // Platform Bottom Collision
                else if (playerBounds.position.y > platformBounds.position.y
                    && playerBounds.position.y + playerBounds.size.y > platformBounds.position.y + platformBounds.size.y
                    && playerBounds.position.x < platformBounds.position.x + platformBounds.size.x
                    && playerBounds.position.x + playerBounds.size.x > platformBounds.position.x)
                {
                    velocity.y = 0.f;
                    player.getHitbox().setPosition({ playerBounds.position.x , platformBounds.position.y + platformBounds.size.y});
                }


                // Platform Left Collision
                if (playerBounds.position.x < platformBounds.position.x 
                    && playerBounds.position.x + playerBounds.size.x < platformBounds.position.x + platformBounds.size.x 
                    && playerBounds.position.y < platformBounds.position.y + platformBounds.size.y
                    && playerBounds.position.y  + playerBounds.size.y > platformBounds.position.y)
                {
                    velocity.x = 0.f;
                    player.getHitbox().setPosition({ platformBounds.position.x - player.getHitbox().getSize().x * astronautScale, playerBounds.position.y });
                }

                // Platform Right Collision
                else if (playerBounds.position.x > platformBounds.position.x
                    && playerBounds.position.x + playerBounds.size.x > platformBounds.position.x + platformBounds.size.x
                    && playerBounds.position.y < platformBounds.position.y + platformBounds.size.y
                    && playerBounds.position.y + playerBounds.size.y > platformBounds.position.y)
                {
                    velocity.x = 0.f;
                    player.getHitbox().setPosition({ platformBounds.position.x + platformBounds.size.x ,playerBounds.position.y });
                }


            }
        }



#pragma endregion

    // Execute movement
    player.getHitbox().move(velocity);
    //astronautSprite.setPosition({ player.getHitbox().getPosition().x - 3 * astronautScale, player.getHitbox().getPosition().y - 6 * astronautScale});
    player.alignPlayerToHitBox(3, 6);

#pragma region Screen Collision

        // Left Collision
        if (player.getHitbox().getPosition().x < 0)
        {
            player.getHitbox().setPosition({ 0, player.getHitbox().getPosition().y });
        }
        // Right Collision
        if (player.getHitbox().getPosition().x > SCREEN_WIDTH - player.getHitbox().getSize().x * astronautScale)
        {
            player.getHitbox().setPosition({ SCREEN_WIDTH - player.getHitbox().getSize().x * astronautScale, player.getHitbox().getPosition().y });
        }
        // Top Collision
        if (player.getHitbox().getPosition().y < 0)
        {
            player.getHitbox().setPosition({ player.getHitbox().getPosition().x, 0 });
        }
        // Bottom Collision
        if (player.getHitbox().getPosition().y > SCREEN_HEIGHT - player.getHitbox().getSize().y * astronautScale)
        {

            player.getHitbox().setPosition({ player.getHitbox().getPosition().x, SCREEN_HEIGHT - player.getHitbox().getSize().y * astronautScale });
        }

#pragma endregion




#pragma region Rendering To Window

        // Render
        window->clear();

        // Drawing
        for (sf::Sprite s : moonSurfaces) 
        {
            window->draw(s);
        }

        for (auto &i : platforms)
        {
            window->draw(i);
        }

        player.drawTo(window);

#pragma endregion
        // Display Window
        window->display();
    }

    delete window;
    return 0;
}