#include "Renderer.h"
#include <SFML/Graphics.hpp>

Renderer::Renderer(sf::RenderTarget& t)
	: target(t)
{
}

void Renderer::Draw(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& size) 
{
	sprite = new sf::Sprite(texture);
	sprite->setPosition(position);
	sprite->setScale(sf::Vector2f(size.x / texture.getSize().x, size.y / texture.getSize().y));
	target.draw(*sprite);
}
