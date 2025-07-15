#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Player {
public:
	bool isJumping = false;
	Player(sf::Vector2f hitBoxSize, sf::Vector2f position, sf::IntRect textureRect, sf::Texture& player_Texture, float Scale);
	// Hitbox
	void alignPlayerToHitBox();
	sf::RectangleShape& getHitbox();
	void showHitBox();

	void drawTo(sf::RenderWindow* window);

	void move(sf::Vector2f distance);
	void setPosition(sf::Vector2f newPos);

	sf::Sprite& getSprite();
	sf::Vector2f getPosition();
	sf::Vector2f getSize();
	sf::Vector2f& getVelocity();

	void setVelocity(sf::Vector2f newVelocity);

	void flipSprite();
	void unflipSprite();

	// Animation
	void resetAnimationFrame();
	void runningAnimation(sf::Clock& animationClock);
	void idleAnimation(sf::Clock& animationClock);
	void jumpingAnimation(sf::Clock& animationClock);

	void setTexture(const sf::Texture& texture)
	{
		playerSprite.setTexture(texture);
	}

private:
	sf::RectangleShape playerHitBox;
	sf::IntRect playerSpriteRect;
	sf::Sprite playerSprite;
	float scale;
	sf::Vector2f velocity;
	sf::Vector2f spriteOffset = { 3, 6 };

};