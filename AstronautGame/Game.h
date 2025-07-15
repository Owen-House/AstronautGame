#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

//extern Camera camera;
void Begin(const sf::Window* window);
void Update(float deltaTime, sf::RenderWindow* window);
void Render(sf::RenderWindow* window);

extern sf::Texture astronautTexture;
extern sf::Texture moonTexture;

extern Player* player;
