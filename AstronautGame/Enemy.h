#pragma once
#include <SFML/Graphics.hpp>
#include "Renderer.h"

class Enemy
{
public:
	float health;
	float damage;
	sf::Vector2f position;
	float speed;
	sf::Texture texture;
	

	Enemy(sf::Texture texture, float health, float damage, sf::Vector2f position, sf::Vector2f size, float speed = 0.f);
	void Draw(Renderer& renderer);

private:
	sf::Vector2f size;
	

};

