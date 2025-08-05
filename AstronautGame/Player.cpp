#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Renderer.h"

//Player::Player()
//{
//
//}

Player::Player(sf::Vector2f hitBoxSize, sf::Vector2f position, sf::IntRect spriteRect, sf::Texture &player_texture, sf::Vector2f size)
	: position(position), size(size), spriteRect(spriteRect), texture(player_texture)
{
	//HitBox
	hitBox.setPosition(position);
	hitBox.setSize(hitBoxSize);
	hitBox.setFillColor(sf::Color::Transparent);
	hitBox.setOutlineColor(sf::Color::White);
	hitBox.setOutlineThickness(-.4f);
	hitBox.setScale(sf::Vector2f({ size.x / spriteRect.size.x, size.y / spriteRect.size.y }));
	
	scale = size.x / spriteRect.size.x;
}

void Player::alignPlayerToHitBox()
{
	position = { hitBox.getPosition().x - offset.x * scale, hitBox.getPosition().y - offset.y * scale };
}

void Player::drawHitBox(sf::RenderWindow* window)
{
	window->draw(hitBox);
}

void Player::Draw(Renderer& renderer)
{
	if (facingLeft)
	{
		flipSprite();
		renderer.Draw(texture, position, { -size.x, size.y }, spriteRect);
	}
	else
	{
		unflipSprite();
		renderer.Draw(texture, position, size, spriteRect);
	}
}

void Player::gatherMovementInputs(float deltaTime, sf::Clock& jumpClock, sf::Clock& animationClock)
{
	bool moving = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W) && jumpClock.getElapsedTime().asSeconds() < maxJumpTime) // Jump
	{
		velocity.y = -speed * deltaTime;
		isJumping = true;
		jumpingAnimation(animationClock);
		if (!jumpClock.isRunning())
		{
			jumpClock.restart();
		}
		moving = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W) && jumpClock.getElapsedTime().asSeconds() >= maxJumpTime) // End jump after maxJumpTime
	{
		isJumping = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) // Move Right
	{
		velocity.x = speed * deltaTime;
		runningAnimation(animationClock);
		moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) // Move Left
	{
		velocity.x = -speed * deltaTime;
		runningAnimation(animationClock);
		moving = true;
	}
	if (!moving) {
		idleAnimation(animationClock);
	}

	// Gravity
	if (position.y < 1080 - size.y && !isJumping)
	{
		velocity.y += 800 * deltaTime;
	}
	else if (!isJumping)
	{
		jumpClock.reset();
	}
}

void Player::move(sf::Vector2f distance)
{
	hitBox.move(distance);
	if (distance.x < 0)
	{
		facingLeft = true;
	}
	else if (distance.x > 0)
	{
		facingLeft = false;
	}
}

void Player::setPosition(sf::Vector2f newPos)
{
	hitBox.setPosition(newPos);
}

sf::RectangleShape& Player::getHitbox()
{
	return hitBox;
}

sf::Vector2f Player::getPosition()
{
	return hitBox.getPosition();
}

sf::Vector2f Player::getSize()
{
	return hitBox.getSize() * scale;
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
	offset = { -12, 6 };
	alignPlayerToHitBox();
}

void Player::unflipSprite()
{
	offset = { 3, 6 };
	alignPlayerToHitBox();
}

void Player::runningAnimation(sf::Clock& animationClock) {
	if (animationClock.getElapsedTime().asSeconds() > .05f)
	{
		spriteRect.position.x = (spriteRect.position.x + 16) % 224;
		spriteRect.position.y = 16;
		animationClock.restart();
	}
}

void Player::idleAnimation(sf::Clock& animationClock)
{
	if (animationClock.getElapsedTime().asSeconds() > .4f)
	{
		spriteRect.position.x = (spriteRect.position.x + 16) % 64;
		spriteRect.position.y = 0;
		animationClock.restart();
	}
}

void Player::jumpingAnimation(sf::Clock& animationClock)
{
	if (animationClock.getElapsedTime().asSeconds() > .2f)
	{
		spriteRect.position.x = (spriteRect.position.x + 16) % 144;
		spriteRect.position.y = 32;
		animationClock.restart();
	}
}