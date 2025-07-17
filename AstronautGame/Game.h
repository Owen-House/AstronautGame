#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

void endOfMovement(const sf::Event::KeyReleased* keyEvent, float deltaTime);

//extern Camera camera;
void Begin(const sf::Window* window);
void Update(float deltaTime, sf::RenderWindow* window);
void Render(sf::RenderWindow* window);


extern Player* player;
