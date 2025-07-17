#pragma once
#include <SFML/Graphics.hpp>


class Renderer
{

public :
	Renderer(sf::RenderTarget& t);
	

	void Draw(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& size);
	~Renderer()
	{
		if (sprite)
			delete sprite;
	}

private:
	sf::Sprite* sprite = nullptr;
	sf::RenderTarget& target;
};

