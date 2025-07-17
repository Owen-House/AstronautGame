#include "Map.h"


Map::Map(float cell_size)
	:cellSize(cell_size), grid()
{
}

void Map::CreateCheckerboard(size_t width, size_t height) 
{
	grid = std::vector(width, std::vector(height, 0));

	bool last = 0;
	for (auto& column : grid)
	{
		for (auto& cell : column)
		{
			cell = !last;
			last = !last;
		}
	}

}

void Map::Draw(sf::RenderWindow* window)
{
	int x = 0;
	for (const auto& column : grid)
	{
		int y = 0;
		for (const auto& cell : column)
		{
			if (cell)
			{
				sf::Sprite block(Resources::textures["Tileset_13.png"]);
				block.setTextureRect(sf::IntRect({ 0,0 }, { 16,16 }));
				block.setPosition({ cellSize * x + cellSize / 2.f, cellSize * y + cellSize / 2.f });
				block.setScale({ cellSize / 16, cellSize / 16});
				window->draw(block);
			}
			y++;

		}
		x++;
	}
	
}