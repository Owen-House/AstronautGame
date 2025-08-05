#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"

class Camera
{
public:
	Camera(float zoomLevel = 5.0f);
	sf::View GetView(sf::Vector2u windowSize);

	void moveWithPlayer(Player* player);

	float zoomLevel;
	sf::Vector2f(position);
	float moveSpeed = 800.f;
};

