#include "Enemy.h"

sf::IntRect animationRect = sf::IntRect({0, 0}, {48, 48});


Enemy::Enemy(sf::Texture texture, float health, float damage, sf::Vector2f position, sf::Vector2f size, float speed) 
	: texture(texture), health(health), damage(damage), position(position), speed(speed), size(size)
{
	
}

void Enemy::Draw(Renderer& renderer)
{
	renderer.Draw(texture, position, size, animationRect);
}