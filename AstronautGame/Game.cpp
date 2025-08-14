#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "Renderer.h"
#include "Resources.h"
#include "Camera.h"
#include "Enemy.h"
#include <filesystem>

unsigned int SCREEN_WIDTH = 1920;
unsigned int SCREEN_HEIGHT = 1080;

float astronautScale = 6.f;

// Gravity
float playerSpeed = 800.0f;
const float groundHeight = float(SCREEN_HEIGHT);
const float gravitySpeed = 800;
 // Seconds

// Clocks

sf::Clock animationClock;

// Player
extern Player* player = nullptr;
sf::Vector2f playerStartPosition = { 80, 600 };

// Enemies
std::vector<Enemy*> enemies;

// Map stuff
std::vector<std::vector<sf::RectangleShape>> blocks;
std::vector<sf::RectangleShape> doors;
float map_cell_size = 54;
Map map(map_cell_size);
Camera camera(1080);

// Menu
bool inMenu = true;

// Levels
int currentLevel = 1;
int maxLevels = 2;
bool changeLevels = false;
static void loadLevel(int level)
{
    std::string levelName = "level" + std::to_string(level) + ".png";
    map.CreateFromImage(Resources::levels[levelName], blocks, playerStartPosition, enemies, doors);
}

void resetGame()
{
    map.resetMap(blocks, enemies, doors);
    loadLevel(currentLevel);
    player->setPosition(playerStartPosition);
    camera.position.x = 960;
    inMenu = true;
}

void changeLevel()
{
    currentLevel = currentLevel % maxLevels + 1;
    loadLevel(currentLevel);
    resetGame();
    changeLevels = false;
    return;
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
    
    // Load Levels
    sf::Image image;
    for (auto& file : std::filesystem::directory_iterator("./resources/levels"))
    {
        if (file.is_regular_file() && (file.path().extension() == ".png" ||
            file.path().extension() == ".jpeg"))
        {
            if (!Resources::levels[file.path().filename().string()].loadFromFile(file.path().string()))
            {
                std::cout << "Failed to load file: " << file.path().filename().string() << std::endl;
                abort();
            }
        }
    }
    loadLevel(currentLevel);

    // Player Setup
    sf::Vector2f hitBoxSize = { 9,10 };
    sf::IntRect textureRect = sf::IntRect({ 0, 16 }, { 16, 16 });
    sf::Vector2f playerSize = { 100, 100 };
    player = new Player(hitBoxSize, playerStartPosition, textureRect, Resources::textures["astronautAnimations.png"], playerSize);

    camera.position = sf::Vector2f({ 960, 540 });
}


void Update(float deltaTime, sf::RenderWindow *window)
{
    if (changeLevels)
        return changeLevel();

    player->gatherMovementInputs(deltaTime, animationClock);
    player->getGridPos(map);
    player->checkCollision(blocks);

    camera.moveWithPlayer(player, map, window->getSize());

    for (auto& door : doors)
    {
        if (player->getHitbox().getGlobalBounds().findIntersection(door.getGlobalBounds()))
        {
            std::cout << "Level Complete!" << std::endl;
            changeLevels = true;
        }
    }
    
    // Execute movement
    player->move(player->getVelocity());

    for (Enemy* e : enemies)
    {
        e->move(deltaTime);
        e->CheckCollision(blocks, deltaTime);
        e->checkPlayerCollision(player);
    }
}


void Render(sf::RenderWindow* window, Renderer& renderer)
{
    map.Draw(renderer);

    player->drawHitBox(window);
    player->Draw(renderer);

    for (Enemy* e : enemies)
    {
        e->Draw(renderer);
        //e->showHitBox(window);
    }

}

