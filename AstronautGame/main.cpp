#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

int main()
{

    
    unsigned int SCREEN_WIDTH= 1920;
    unsigned int SCREEN_HEIGHT = 1080;

    float astronautScale = 6.f;
    float numSurfaces = 2.0;
    sf::Clock clock;

    const float gridSize = 150.f;

    sf::Vector2f mousePosGrid;

    float playerSpeed = 800.0;

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

#pragma region Sprites

    

    sf::Sprite astronautSprite(astronautTexture);
    sf::Vector2f astronautOrigin = { 0.0, 0.0 };
    astronautSprite.setTextureRect(sf::IntRect({ 0, 0 }, { 16, 16 }));
    astronautSprite.setOrigin(astronautOrigin);
    astronautSprite.setPosition({ 80, float(SCREEN_HEIGHT - 300)});
    astronautSprite.setScale(sf::Vector2f({ astronautScale, astronautScale }));

    std::vector<sf::Sprite> moonSurfaces;
    for (auto i = 0; i < numSurfaces; i++) 
    {
        sf::Sprite moonSprite(moonTexture);
        moonSprite.setOrigin({ 0, 32 });
        moonSprite.setPosition({ float(i * (SCREEN_WIDTH / numSurfaces)), float(SCREEN_HEIGHT)});
        moonSprite.setScale(sf::Vector2f({ 7.5, 7.5 }));
        moonSurfaces.push_back(moonSprite);
    }
    


#pragma endregion


    // Walls
    std::vector<sf::RectangleShape> platforms;

    sf::RectangleShape platform;
    platform.setFillColor(sf::Color::Red);
    platform.setSize(sf::Vector2f(gridSize, gridSize));
    platform.setPosition(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));

    //platforms.push_back(platform);
    
    // Game Clock

    // Collision Stuff
    sf::FloatRect nextPos;
    sf::RectangleShape astronautHitBox;
    astronautHitBox.setSize(sf::Vector2f(9, 10));
    astronautHitBox.setScale(sf::Vector2f({ astronautScale, astronautScale }));
    astronautHitBox.setFillColor(sf::Color::Transparent);
    astronautHitBox.setPosition({ astronautSprite.getPosition().x + 3 * astronautScale,
        astronautSprite.getPosition().y + 6 * astronautScale });
    
    // Show hitbox
    //astronautHitBox.setOutlineColor(sf::Color::White);
    //astronautHitBox.setOutlineThickness(.1f);


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
            sf::FloatRect playerBounds = astronautHitBox.getGlobalBounds();
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
                    astronautHitBox.setPosition({playerBounds.position.x, platformBounds.position.y - playerBounds.size.y});
                }

                // Platform Bottom Collision
                else if (playerBounds.position.y > platformBounds.position.y
                    && playerBounds.position.y + playerBounds.size.y > platformBounds.position.y + platformBounds.size.y
                    && playerBounds.position.x < platformBounds.position.x + platformBounds.size.x
                    && playerBounds.position.x + playerBounds.size.x > platformBounds.position.x)
                {
                    velocity.y = 0.f;
                    astronautHitBox.setPosition({ playerBounds.position.x , platformBounds.position.y + platformBounds.size.y});
                }


                // Platform Left Collision
                if (playerBounds.position.x < platformBounds.position.x 
                    && playerBounds.position.x + playerBounds.size.x < platformBounds.position.x + platformBounds.size.x 
                    && playerBounds.position.y < platformBounds.position.y + platformBounds.size.y
                    && playerBounds.position.y  + playerBounds.size.y > platformBounds.position.y)
                {
                    velocity.x = 0.f;
                    astronautHitBox.setPosition({ platformBounds.position.x - astronautHitBox.getSize().x * astronautScale, playerBounds.position.y });
                }

                // Platform Right Collision
                else if (playerBounds.position.x > platformBounds.position.x
                    && playerBounds.position.x + playerBounds.size.x > platformBounds.position.x + platformBounds.size.x
                    && playerBounds.position.y < platformBounds.position.y + platformBounds.size.y
                    && playerBounds.position.y + playerBounds.size.y > platformBounds.position.y)
                {
                    velocity.x = 0.f;
                    astronautHitBox.setPosition({ platformBounds.position.x + platformBounds.size.x ,playerBounds.position.y });
                }


            }
        }



#pragma endregion

    // Execute movement
    astronautHitBox.move(velocity);
    astronautSprite.setPosition({ astronautHitBox.getPosition().x - 3 * astronautScale, astronautHitBox.getPosition().y - 6 * astronautScale});

#pragma region Screen Collision

        // Left Collision
        if (astronautHitBox.getPosition().x < 0)
        {
            astronautHitBox.setPosition({ 0, astronautHitBox.getPosition().y });
        }
        // Right Collision
        if (astronautHitBox.getPosition().x > SCREEN_WIDTH - astronautHitBox.getSize().x * astronautScale)
        {
            astronautHitBox.setPosition({ SCREEN_WIDTH - astronautHitBox.getSize().x * astronautScale, astronautHitBox.getPosition().y });
        }
        // Top Collision
        if (astronautHitBox.getPosition().y < 0)
        {
            astronautHitBox.setPosition({ astronautHitBox.getPosition().x, 0 });
        }
        // Bottom Collision
        if (astronautHitBox.getPosition().y > SCREEN_HEIGHT - astronautHitBox.getSize().y * astronautScale)
        {
            astronautHitBox.setPosition({ astronautHitBox.getPosition().x, SCREEN_HEIGHT - astronautHitBox.getSize().y * astronautScale });
        }

#pragma endregion





        // Render
        window->clear();

        // Drawing
        for (sf::Sprite s : moonSurfaces) 
        {
            window->draw(s);
        }

        window->draw(astronautSprite);

        for (auto &i : platforms)
        {
            window->draw(i);
        }

        window->draw(astronautHitBox);

        // Display Window
        window->display();
    }

    delete window;
    return 0;
}