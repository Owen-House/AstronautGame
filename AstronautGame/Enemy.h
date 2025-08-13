#pragma once
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "Player.h"
#include "Game.h"
#include <iostream>

class Enemy
{
public:
	float health;
	float damage;
	sf::Vector2f position;
	float speed;
	sf::Texture texture;
	sf::RectangleShape hitBox;
	int yGridPos;

	Enemy(sf::Vector2f hitBoxSize, sf::Texture texture, float health, float damage,
		sf::Vector2f position, sf::Vector2f size, int yGridPos, float speed = 0.f);

	void Draw(Renderer& renderer);
	void CheckCollision(const std::vector< std::vector<sf::RectangleShape>>& blocks, float deltaTime);
	void move(float deltaTime);
	bool checkPlayerCollision(Player* player);

	void showHitBox(sf::RenderWindow* window){ window->draw(hitBox); }

	void faceLeft();
	void faceRight();
	
private:
	sf::Vector2f size;
	bool isFacingLeft = true;

	//Hitbox
	float scale = 6.f;
	sf::Vector2f offset = { 3.f, 6.f };
	void alignToHitBox();

	
};

