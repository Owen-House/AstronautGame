#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Player {
public:

	Player(sf::Vector2f hitBoxSize, sf::Vector2f position, sf::IntRect textureRect, sf::Sprite player_sprite, float Scale)
		: playerSprite(player_sprite), scale(Scale), playerSpriteRect(textureRect)
	{
		//Sprite
		playerSprite.setPosition(position);
		playerSprite.setScale({ scale , scale});
		playerSprite.setTextureRect(textureRect);

		//HitBox
		playerHitBox.setSize(hitBoxSize);
		playerHitBox.setFillColor(sf::Color::Transparent);
		playerHitBox.setSize(hitBoxSize);
		playerHitBox.setScale(sf::Vector2f({ scale, scale }));

	}

	void alignPlayerToHitBox()
	{
		playerSprite.setPosition({ playerHitBox.getPosition().x - spriteOffset.x * scale, playerHitBox.getPosition().y - spriteOffset.y * scale });
	}

	void drawTo(sf::RenderWindow *window)
	{
		window->draw(playerHitBox);
		window->draw(playerSprite);
	}

	void move(sf::Vector2f distance)
	{
		playerHitBox.move(distance);
	}

	void setPosition(sf::Vector2f newPos)
	{
		playerHitBox.setPosition(newPos);
	}

	sf::RectangleShape& getHitbox()
	{
		return playerHitBox;
	}

	sf::Sprite& getSprite()
	{
		return playerSprite;
	}

	void showHitBox()
	{
		playerHitBox.setOutlineColor(sf::Color::White);
		playerHitBox.setOutlineThickness(-.4);
	}

	sf::Vector2f getPosition()
	{
		return playerHitBox.getPosition();
	}

	sf::Vector2f getSize()
	{
		return playerHitBox.getSize() * scale;
	}

	sf::Vector2f &getVelocity()
	{
		return velocity;
	}

	void setVelocity(sf::Vector2f newVelocity)
	{
		velocity = newVelocity;
	}

	void flipSprite()
	{
		playerSprite.setScale({ -scale, scale });
		spriteOffset = { -12, 6 };
		alignPlayerToHitBox();
	}

	void unflipSprite()
	{
		playerSprite.setScale({ scale, scale });
		spriteOffset = { 3, 6 };
		alignPlayerToHitBox();
	}

	void resetAnimationFrame()
	{
		playerSpriteRect.position.x = 0;
		playerSpriteRect.position.y = 0;
		playerSprite.setTextureRect(playerSpriteRect);
	}

	void runningAnimation(sf::Clock &animationClock) {
		if (animationClock.getElapsedTime().asSeconds() > .1f)
		{
			playerSpriteRect.position.x = (playerSpriteRect.position.x + 16) % 224;
			playerSpriteRect.position.y = 16;
			playerSprite.setTextureRect(playerSpriteRect);
			animationClock.restart();
		}
	}

	void idleAnimation(sf::Clock& animationClock)
	{
		if (animationClock.getElapsedTime().asSeconds() > .5f)
		{
			playerSpriteRect.position.x = (playerSpriteRect.position.x + 16) % 64;
			playerSpriteRect.position.y = 0;
			playerSprite.setTextureRect(playerSpriteRect);
			animationClock.restart();
		}
	}

	void jumpingAnimation(sf::Clock& animationClock)
	{
		if (animationClock.getElapsedTime().asSeconds() > .1f)
		{
			playerSpriteRect.position.x = (playerSpriteRect.position.x + 16) % 144;
			playerSpriteRect.position.y = 32;
			playerSprite.setTextureRect(playerSpriteRect);
			animationClock.restart();
		}
	}

private:
	sf::RectangleShape playerHitBox;
	sf::IntRect playerSpriteRect;
	sf::Sprite playerSprite;
	float scale;
	sf::Vector2f velocity;
	sf::Vector2f spriteOffset = { 3, 6 };

};