#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"

class Map;

class Camera
{
public:
	Camera(float zoomLevel = 5.0f);
	sf::View GetView(sf::Vector2u windowSize);

	void moveWithPlayer(Player* player, Map& map, sf::Vector2u windowSize);

	float zoomLevel;
	sf::Vector2f position;
	float moveSpeed = 800.f;
};

