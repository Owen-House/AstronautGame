#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "Renderer.h"
#include "Resources.h"
#include <filesystem>

unsigned int SCREEN_WIDTH = 1920;
unsigned int SCREEN_HEIGHT = 1080;

float astronautScale = 6.f;
float numGroundSurfaces = 2.0f;

const float gridSize = 150.f;
sf::Vector2f mousePosGrid;

sf::FloatRect nextPos;

// Gravity
float playerSpeed = 800.0f;
const float groundHeight = float(SCREEN_HEIGHT - 80);
const float gravitySpeed = 800;
float maxJumpTime = 0.3f; // Seconds

// Clocks
sf::Clock jumpClock;
sf::Clock animationClock;

// Player
extern Player* player = nullptr;

std::vector<sf::RectangleShape> platforms;
sf::RectangleShape platform;
std::vector<sf::Sprite> moonSurfaces;

//Map stuff
Map map;

void endOfMovement(const sf::Event::KeyReleased* keyEvent, float deltaTime)
{
    // Change velocity based on key released
    if (keyEvent->code == sf::Keyboard::Key::W && player->getVelocity().y > 0)
    {
        std::cout << "W";
        player->isJumping = false;
        player->setVelocity({ player->getVelocity().x, 0.f });
    }
    else if (keyEvent->code == sf::Keyboard::Key::A && player->getVelocity().x < 0)
    {
        std::cout << "A";
        player->setVelocity({ 0.f, player->getVelocity().y});
    }
    else if (keyEvent->code == sf::Keyboard::Key::D && player->getVelocity().x > 0)
    {
        std::cout << "D";
        player->setVelocity({ 0.f, player->getVelocity().y });
    }
}

void Begin(const sf::Window* window)
{
    // Load textures
    for (auto& file : std::filesystem::directory_iterator("./resources/textures"))
    {
        if (file.is_regular_file() && (file.path().extension() == ".png" ||
            file.path().extension() == ".jpeg"))
        {
            Resources::textures[file.path().filename().string()].loadFromFile(file.path().string());
        }
    }
    
    // Player Setup
    player = new Player({ 9, 10 }, { 1000, 1000 }, sf::IntRect({ 0, 16 }, { 16, 16 }), Resources::textures["astronautAnimations.png"], astronautScale);
    player->alignPlayerToHitBox();
    //player->showHitBox();

	jumpClock.reset();

    // Load Ground
    for (auto i = 0; i < numGroundSurfaces; i++)
    {
        sf::Sprite moonSprite(Resources::textures["MoonSurface.png"]);
        moonSprite.setOrigin({ 0, 32 });
        moonSprite.setPosition({ float(i * (SCREEN_WIDTH / numGroundSurfaces)), float(SCREEN_HEIGHT) });
        moonSprite.setScale(sf::Vector2f({ 7.5, 7.5 }));
        moonSurfaces.push_back(moonSprite);
    }

    // Platforms
    platform.setFillColor(sf::Color::Red);
    platform.setSize(sf::Vector2f(gridSize, gridSize));
    platform.setPosition(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));

    map.CreateCheckerboard(11, 11);
}


void Update(float deltaTime, sf::RenderWindow *window)
{
#pragma region Preparing Movement 

    bool moving = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W) && jumpClock.getElapsedTime().asSeconds() < maxJumpTime) // Jump
    {
        player->getVelocity().y = -playerSpeed * deltaTime;
        player->isJumping = true;
        player->jumpingAnimation(animationClock);
        if (!jumpClock.isRunning())
        {
            jumpClock.restart();
        }
        moving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W) && jumpClock.getElapsedTime().asSeconds() >= maxJumpTime)
    {
        player->isJumping = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) // Move Right
    {
        player->getVelocity().x = playerSpeed * deltaTime;
        player->unflipSprite();
        player->runningAnimation(animationClock);
        moving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) // Move Left
    {
        player->getVelocity().x = -playerSpeed * deltaTime;
        player->flipSprite();
        player->runningAnimation(animationClock);
        moving = true;
    }
    if (!moving) {
        player->idleAnimation(animationClock);
    }

    // Gravity
    if (player->getPosition().y < groundHeight - player->getSize().y && !player->isJumping)
    {
        player->getVelocity().y += gravitySpeed * deltaTime;
    }
    else if (!player->isJumping)
    {
        jumpClock.reset();
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
            platform.setPosition({ mousePosGrid.x * gridSize, mousePosGrid.y * gridSize });
            platforms.push_back(platform);
        }
    }

    for (auto& platform : platforms)
    {
        sf::FloatRect playerBounds = player->getHitbox().getGlobalBounds();
        sf::FloatRect platformBounds = platform.getGlobalBounds();

        nextPos = playerBounds;
        nextPos.position.x += player->getVelocity().x;
        nextPos.position.y += player->getVelocity().y;



        if (platformBounds.findIntersection(nextPos))
        {

            // Platform Top Collision
            if (playerBounds.position.y < platformBounds.position.y
                && playerBounds.position.y + playerBounds.size.y < platformBounds.position.y + platformBounds.size.y
                && playerBounds.position.x < platformBounds.position.x + platformBounds.size.x
                && playerBounds.position.x + playerBounds.size.x > platformBounds.position.x)
            {
                player->getVelocity().y = 0.f;
                player->setPosition({ playerBounds.position.x, platformBounds.position.y - playerBounds.size.y });
                jumpClock.restart();
                player->isJumping = false;
            }

            // Platform Bottom Collision
            else if (playerBounds.position.y > platformBounds.position.y
                && playerBounds.position.y + playerBounds.size.y > platformBounds.position.y + platformBounds.size.y
                && playerBounds.position.x < platformBounds.position.x + platformBounds.size.x
                && playerBounds.position.x + playerBounds.size.x > platformBounds.position.x)
            {
                player->getVelocity().y = 0.f;
                player->setPosition({ playerBounds.position.x , platformBounds.position.y + platformBounds.size.y });
            }


            // Platform Left Collision
            if (playerBounds.position.x < platformBounds.position.x
                && playerBounds.position.x + playerBounds.size.x < platformBounds.position.x + platformBounds.size.x
                && playerBounds.position.y < platformBounds.position.y + platformBounds.size.y
                && playerBounds.position.y + playerBounds.size.y > platformBounds.position.y)
            {
                player->getVelocity().x = 0.f;
                player->setPosition({ platformBounds.position.x - player->getSize().x, playerBounds.position.y });
            }

            // Platform Right Collision
            else if (playerBounds.position.x > platformBounds.position.x
                && playerBounds.position.x + playerBounds.size.x > platformBounds.position.x + platformBounds.size.x
                && playerBounds.position.y < platformBounds.position.y + platformBounds.size.y
                && playerBounds.position.y + playerBounds.size.y > platformBounds.position.y)
            {
                player->getVelocity().x = 0.f;
                player->setPosition({ platformBounds.position.x + platformBounds.size.x ,playerBounds.position.y });
            }


        }
    }

#pragma endregion

    // Execute movement
    player->move(player->getVelocity());
    player->alignPlayerToHitBox();

#pragma region Screen Collision

    // Left Collision
    if (player->getPosition().x < 0)
    {
        player->setPosition({ 0, player->getPosition().y });
    }
    // Right Collision
    if (player->getPosition().x > SCREEN_WIDTH - player->getSize().x)
    {
        player->setPosition({ SCREEN_WIDTH - player->getSize().x, player->getPosition().y });
    }
    // Top Collision
    if (player->getPosition().y < 0)
    {
        player->setPosition({ player->getPosition().x, 0 });
    }

#pragma endregion

#pragma region Map

    map.CreateCheckerboard(11, 11);

#pragma endregion

}


void Render(sf::RenderWindow* window)
{
    window->clear();

    map.Draw(window);

    for (sf::Sprite s : moonSurfaces)
    {
        window->draw(s);
    }
    for (auto& i : platforms)
    {
        window->draw(i);
    }

    player->drawTo(window);


    window->display();
}