#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Resources.h"
#include "Renderer.h"
#include "Enemy.h"

class Map
{
public:
	Map(float cell_size = 128.0f);
	
	void CreateCheckerboard(size_t width, size_t height);
	void CreateFromImage(const sf::Image& image, std::vector<sf::RectangleShape>& blocks, sf::Vector2f& playerStartPosition, std::vector<Enemy*>& enemies);

	void Draw(Renderer& renderer);

	std::vector<std::vector<int>> grid;
	float cellSize;
	sf::RectangleShape block;
};