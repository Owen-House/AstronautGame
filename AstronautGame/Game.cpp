#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "Renderer.h"
#include "Resources.h"
#include "Camera.h"
#include "Enemy.h"
#include "MainMenu.h"
#include <filesystem>

unsigned int SCREEN_WIDTH = 1920;
unsigned int SCREEN_HEIGHT = 1080;

float astronautScale = 6.f;

// Gravity
float playerSpeed = 800.0f;
const float groundHeight = float(SCREEN_HEIGHT);
const float gravitySpeed = 800;
float maxJumpTime = 0.3f; // Seconds

// Clocks
sf::Clock jumpClock;
sf::Clock animationClock;

// Player
extern Player* player = nullptr;
sf::Vector2f playerStartPosition = { 80, 600 };
sf::FloatRect nextPos;

// Enemies
std::vector<Enemy*> enemies;

// Map stuff
std::vector<sf::RectangleShape> blocks;
float map_cell_size = 54.f;
Map map(map_cell_size);
Camera camera(1080);

// Menu
bool inMenu = true;

void resetGame()
{
    player->setPosition(playerStartPosition);
    camera.position.x = 960;
    inMenu = true;
}

void Begin(const sf::Window* window)
{

    // Load textures and fonts
    for (auto& file : std::filesystem::directory_iterator("./resources/textures"))
    {
        if (file.is_regular_file() && (file.path().extension() == ".png" ||
            file.path().extension() == ".jpeg"))
        {
            if (!Resources::textures[file.path().filename().string()].loadFromFile(file.path().string()))
            {
                std::cout << "Failed to load file: "<< file.path().filename().string() << std::endl;
                abort();
            }
        }
        if (file.is_regular_file() && (file.path().extension() == ".ttf"))
        {
            if (!Resources::fonts[file.path().filename().string()].openFromFile(file.path().string()))
            {
                std::cout << "Failed to load file: " << file.path().filename().string() << std::endl;
                abort();
            }
        }
    }
    
    // Map Setup
    sf::Image image;
    if (!image.loadFromFile("resources/map2.png"))
    {
        std::cout << "Could not load resources/map.png" << std::endl;
        std::abort();
    }
    map.CreateFromImage(image, blocks, playerStartPosition);

    // Player Setup
    sf::Vector2f hitBoxSize = { 9,10 };
    
    sf::IntRect textureRect = sf::IntRect({ 0, 16 }, { 16, 16 });
    sf::Vector2f playerSize = { 100, 100 };
    player = new Player(hitBoxSize, playerStartPosition, textureRect, Resources::textures["astronautAnimations.png"], playerSize);

    // Enemies Setup
    Enemy* enemy = new Enemy(hitBoxSize, Resources::textures["Alien.png"], 10.f, 10.f, { 800, 900 }, {100, 100}, 300.f);
    Enemy* enemy2 = new Enemy(hitBoxSize, Resources::textures["Alien.png"], 10.f, 10.f, { 3000, 900 }, { 100, 100 }, 300.f);

    enemies.push_back(enemy);
    enemies.push_back(enemy2);


    player->alignPlayerToHitBox();

	jumpClock.reset();

    camera.position = sf::Vector2f({ 960, 540 });
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
        //player->unflipSprite();
        player->runningAnimation(animationClock);
        moving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) // Move Left
    {
        player->getVelocity().x = -playerSpeed * deltaTime;
        //player->flipSprite();
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

#pragma region Camera Movement

    // Automatic Camera Movement
    if (camera.position.x > player->getPosition().x + 300 && camera.position.x > 960)
    {
        // Move Camera Left
        camera.position.x -= camera.moveSpeed * deltaTime;
    }
    if ((camera.position.x < player->getPosition().x - 300))
    {
        // Move Camera Right
        camera.position.x += camera.moveSpeed * deltaTime;
    }
    

#pragma endregion

    
#pragma region Platform Collision

    
    for (auto& block : blocks)
    {
        sf::FloatRect playerBounds = player->getHitbox().getGlobalBounds();
        sf::FloatRect platformBounds = block.getGlobalBounds();

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

#pragma region Enemies

    for (Enemy* e : enemies)
    {
        e->move(deltaTime);
        e->CheckCollision(blocks, deltaTime);
        e->checkPlayerCollision(player);
    }

#pragma endregion


#pragma region Screen Collision

    // Left Collision
    if (player->getPosition().x < 0)
    {
        player->setPosition({ 0, player->getPosition().y });
    }
    // Top Collision
    if (player->getPosition().y < 0)
    {
        player->setPosition({ player->getPosition().x, 0 });
    }

#pragma endregion


}


void Render(sf::RenderWindow* window, Renderer& renderer)
{
    map.Draw(renderer);

    //player->drawHitBox(window);
    player->Draw(renderer);

    for (Enemy* e : enemies)
    {
        e->Draw(renderer);
        //e->showHitBox(window);
    }

    //menu.draw(window);
}

