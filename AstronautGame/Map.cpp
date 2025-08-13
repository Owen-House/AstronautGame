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

void Map::resetMap(std::vector< std::vector<sf::RectangleShape>>& blocks, std::vector<Enemy*>& enemies, std::vector<sf::RectangleShape>& doors)
{
	grid.clear();
	for (unsigned int i = 0; i < blocks.size(); i++)
	{
		blocks[i].clear();
	}
	blocks.clear();
	doors.clear();
	for (Enemy* e : enemies)
	{
		delete e;
	}
	enemies.clear();
}

void Map::CreateFromImage(const sf::Image& image, std::vector< std::vector<sf::RectangleShape>>& blocks,
	sf::Vector2f& playerStartPosition, std::vector<Enemy*>& enemies, std::vector<sf::RectangleShape>& doors)
{
	resetMap(blocks, enemies, doors);
	grid = std::vector(image.getSize().x, std::vector(image.getSize().y, 0));

	blocks.resize(image.getSize().y);

	for (size_t x = 0; x < grid.size(); x++)
	{
		for (size_t y = 0; y < grid[x].size(); y++)
		{
			sf::Color color = image.getPixel({ x, y });
			if (color == sf::Color::Black) // Blocks
			{
				grid[x][y] = 1;
				block.setPosition({ cellSize * (x - 1) , cellSize * y });
				blocks[y].push_back(block);
			}
			else if (color == sf::Color::Red) // Player
			{
				playerStartPosition = { cellSize * (x - 1), cellSize * y - 10 };
			}
			else if (color == sf::Color::Blue) // Enemies
			{
				sf::Vector2f hitBoxSize = { 9,10 };
				Enemy* enemy = new Enemy(hitBoxSize, Resources::textures["Alien.png"], 10.f, 10.f, { cellSize * (x - 1), cellSize * y - 10 }, { 100, 100 }, y, 300.f);
				enemies.push_back(enemy);
			}
			else if (color == sf::Color::Yellow) // Doors
			{
				grid[x][y] = 2;
				block.setPosition({cellSize * (x - 1) , cellSize * y});
				doors.push_back(block);
			}
		}
		std::cout << x << std::endl;
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
			switch (cell)
			{
			case 1:
				renderer.Draw(Resources::textures["Block.png"], { cellSize * (x - 1) , cellSize * y },
					{ cellSize, cellSize }, sf::IntRect({ 1,1 }, { 14, 14 }));
				break;
			case 2:
				renderer.Draw(Resources::textures["Door.png"], { cellSize * (x - 1) , cellSize * y },
					{ cellSize, cellSize }, sf::IntRect({ 1,1 }, { 14, 14 }));
				break;
			}
			y++;

		}
		
		x++;
	}
	
}