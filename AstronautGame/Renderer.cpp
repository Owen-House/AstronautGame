#include "Renderer.h"
#include <SFML/Graphics.hpp>

Renderer::Renderer(sf::RenderTarget& t)
	: target(t)
{
}

void Renderer::Draw(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& size, const sf::IntRect& textureRect)
{
	sf::Sprite sprite(texture);
	sprite.setPosition(position);
	sprite.setScale(sf::Vector2f(size.x / textureRect.size.x, size.y / textureRect.size.y));
	sprite.setTextureRect(textureRect);
	target.draw(sprite);
}
