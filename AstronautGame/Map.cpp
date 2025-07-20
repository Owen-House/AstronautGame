#include "Map.h"


Map::Map(float cell_size)
	:cellSize(cell_size), grid()
{
	block.setFillColor(sf::Color::Transparent);
	block.setSize({ cellSize, cellSize });
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

void Map::CreateFromImage(const sf::Image& image, std::vector<sf::RectangleShape>& blocks)
{
	grid.clear();
	grid = std::vector(image.getSize().x, std::vector(image.getSize().y, 0));

	for (size_t x = 0; x < grid.size(); x++)
	{
		for (size_t y = 0; y < grid[x].size(); y++)
		{
			sf::Color color = image.getPixel({ x, y });
			if (color != sf::Color::White)
			{
				grid[x][y] = 1;
				block.setPosition({ cellSize * x , cellSize * y });
				blocks.push_back(block);
			}
		}
	}
}


void Map::Draw(Renderer& renderer)
{
	int x = 0;
	for (const auto& column : grid)
	{
		int y = 0;
		for (const auto& cell : column)
		{
			if (cell)
			{
				renderer.Draw(Resources::textures["Tileset_13.png"], { cellSize * x , cellSize * y  },
					{ cellSize, cellSize }, sf::IntRect({ 1,1 }, { 14, 14 }));
			}
			y++;

		}
		x++;
	}
	
}