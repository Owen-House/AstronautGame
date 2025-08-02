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
	

	Enemy(sf::Vector2f hitBoxSize, sf::Texture texture, float health, float damage, sf::Vector2f position, sf::Vector2f size, float speed = 0.f);
	
	void Draw(Renderer& renderer);
	
	void CheckCollision(std::vector<sf::RectangleShape> blocks, float deltaTime);

	void move(float deltaTime);
	void showHitBox(sf::RenderWindow* window)
	{
		window->draw(hitBox);
	}
	
private:
	sf::Vector2f size;
	
	//Hitbox
	float scale = 6.f;
	sf::RectangleShape hitBox;
	sf::Vector2f offset = { 3.f, 6.f };
	void alignToHitBox();

	
};

