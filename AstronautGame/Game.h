#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Renderer.h"
#include "Camera.h"

void endOfMovement(const sf::Event::KeyReleased* keyEvent, float deltaTime);

extern Camera camera;

void Begin(const sf::Window* window);
void Update(float deltaTime, sf::RenderWindow* window);
void Render(sf::RenderWindow* window, Renderer& renderer);


extern Player* player;
