#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"

//Player::Player()
//{
//
//}

Player::Player(sf::Vector2f hitBoxSize, sf::Vector2f position, sf::IntRect textureRect, sf::Texture &player_texture, float Scale)
	: playerSprite(sf::Sprite(player_texture)), scale(Scale), playerSpriteRect(textureRect)
{
	//Sprite
	playerSprite.setPosition(position);
	playerSprite.setScale({ scale , scale });
	playerSprite.setTextureRect(textureRect);

	//HitBox
	playerHitBox.setSize(hitBoxSize);
	playerHitBox.setFillColor(sf::Color::Transparent);
	playerHitBox.setSize(hitBoxSize);
	playerHitBox.setScale(sf::Vector2f({ scale, scale }));

}

void Player::alignPlayerToHitBox()
{
	playerSprite.setPosition({ playerHitBox.getPosition().x - spriteOffset.x * scale, playerHitBox.getPosition().y - spriteOffset.y * scale });
}

void Player::drawTo(sf::RenderWindow* window)
{
	window->draw(playerHitBox);
	window->draw(playerSprite);
}

void Player::move(sf::Vector2f distance)
{
	playerHitBox.move(distance);
}

void Player::setPosition(sf::Vector2f newPos)
{
	playerHitBox.setPosition(newPos);
}

sf::RectangleShape& Player::getHitbox()
{
	return playerHitBox;
}

sf::Sprite& Player::getSprite()
{
	return playerSprite;
}

void Player::showHitBox()
{
	playerHitBox.setOutlineColor(sf::Color::White);
	playerHitBox.setOutlineThickness(-.4f);
}

sf::Vector2f Player::getPosition()
{
	return playerHitBox.getPosition();
}

sf::Vector2f Player::getSize()
{
	return playerHitBox.getSize() * scale;
}

sf::Vector2f& Player::getVelocity()
{
	return velocity;
}

void Player::setVelocity(sf::Vector2f newVelocity)
{
	velocity = newVelocity;
}

void Player::flipSprite()
{
	playerSprite.setScale({ -scale, scale });
	spriteOffset = { -12, 6 };
	alignPlayerToHitBox();
}

void Player::unflipSprite()
{
	playerSprite.setScale({ scale, scale });
	spriteOffset = { 3, 6 };
	alignPlayerToHitBox();
}

void Player::resetAnimationFrame()
{
	playerSpriteRect.position.x = 0;
	playerSpriteRect.position.y = 0;
	playerSprite.setTextureRect(playerSpriteRect);
}

void Player::runningAnimation(sf::Clock& animationClock) {
	if (animationClock.getElapsedTime().asSeconds() > .1f)
	{
		playerSpriteRect.position.x = (playerSpriteRect.position.x + 16) % 224;
		playerSpriteRect.position.y = 16;
		playerSprite.setTextureRect(playerSpriteRect);
		animationClock.restart();
	}
}

void Player::idleAnimation(sf::Clock& animationClock)
{
	if (animationClock.getElapsedTime().asSeconds() > .5f)
	{
		playerSpriteRect.position.x = (playerSpriteRect.position.x + 16) % 64;
		playerSpriteRect.position.y = 0;
		playerSprite.setTextureRect(playerSpriteRect);
		animationClock.restart();
	}
}

void Player::jumpingAnimation(sf::Clock& animationClock)
{
	if (animationClock.getElapsedTime().asSeconds() > .1f)
	{
		playerSpriteRect.position.x = (playerSpriteRect.position.x + 16) % 144;
		playerSpriteRect.position.y = 32;
		playerSprite.setTextureRect(playerSpriteRect);
		animationClock.restart();
	}
}