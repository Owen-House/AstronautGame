#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

int main()
{

    //Open Window
    unsigned int SCREEN_WIDTH= 1920;
    unsigned int SCREEN_HEIGHT = 1080;
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }), "AstronautGame");
    window->setFramerateLimit(120);



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
    sf::Vector2f astronautOrigin = { 8.0f, 8.0f };
    astronautSprite.setTextureRect(sf::IntRect({ 16, 0 }, { 16, 16 }));
    astronautSprite.setOrigin(astronautOrigin);
    astronautSprite.setPosition({ 80, float(SCREEN_HEIGHT - 180)});
    astronautSprite.setScale(sf::Vector2f({ 6, 6 }));

    float numSurfaces = 2.0;
    std::vector<sf::Sprite> moonSurfaces;
    for (auto i = 0; i < numSurfaces; i++) 
    {
        sf::Sprite moonSprite(moonTexture);
        moonSprite.setOrigin({ 0, 32 });
        moonSprite.setPosition({ i * (1920 / numSurfaces), float(SCREEN_HEIGHT)});
        moonSprite.setScale(sf::Vector2f({ 7.5, 7.5 }));
        moonSurfaces.push_back(moonSprite);
    }
    


#pragma endregion

    
    // Game Clock
    sf::Clock clock;

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


#pragma region Movement

        float playerSpeed = 800.0;
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

        astronautSprite.move(velocity);
#pragma endregion


#pragma region ScreenCollision

        // Left Collision
        if (astronautSprite.getPosition().x < astronautOrigin.x + 20)
        {
            astronautSprite.setPosition({ astronautOrigin.x + 20, astronautSprite.getPosition().y });
        }
        // Right Collision
        if (astronautSprite.getPosition().x > SCREEN_WIDTH - astronautOrigin.x - 20)
        {
            astronautSprite.setPosition({ SCREEN_WIDTH - astronautOrigin.x - 20, astronautSprite.getPosition().y });
        }
        // Top Collision
        if (astronautSprite.getPosition().y < astronautOrigin.y + 20)
        {
            astronautSprite.setPosition({ astronautSprite.getPosition().x, astronautOrigin.y + 20 });
        }
        // Bottom Collision
        if (astronautSprite.getPosition().y > SCREEN_HEIGHT - astronautOrigin.y - 120)
        {
            astronautSprite.setPosition({ astronautSprite.getPosition().x, SCREEN_HEIGHT - astronautOrigin.y - 120});
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


        // Display Window
        window->display();

    }

    delete window;
    return 0;
}