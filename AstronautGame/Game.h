#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Renderer.h"
#include "Camera.h"


extern Camera camera;

void Begin(const sf::Window* window);
void Update(float deltaTime, sf::RenderWindow* window);
void Render(sf::RenderWindow* window, Renderer& renderer);

void resetGame();

extern bool inMenu;
extern Player* player;
extern int currentLevel;
