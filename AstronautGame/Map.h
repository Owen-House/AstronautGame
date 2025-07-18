#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Resources.h"
#include "Renderer.h"

class Map
{
public:
	Map(float cell_size = 128.0f);
	void CreateCheckerboard(size_t width, size_t height);
	void Draw(Renderer& renderer);

	std::vector<std::vector<int>> grid;
	float cellSize;
};