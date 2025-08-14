#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Map.h"


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

	jumpClock.reset();
	alignPlayerToHitBox();
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

void Player::gatherMovementInputs(float deltaTime, sf::Clock& animationClock)
{
	bool moving = false;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W) && jumpClock.getElapsedTime().asSeconds() < maxJumpTime) // Jump
	{
		if (isJumping || onGround)
		{
			onGround = false;
			velocity.y = -speed * deltaTime;
			isJumping = true;
			jumpingAnimation(animationClock);
			moving = true;
			if (!jumpClock.isRunning())
				jumpClock.restart();
		}	
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W) && jumpClock.getElapsedTime().asSeconds() >= maxJumpTime) // End jump after maxJumpTime
	{
		isJumping = false;

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) // Move Right
	{
		velocity.x += speed * deltaTime;
		runningAnimation(animationClock);
		moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) // Move Left
	{
		velocity.x += -speed * deltaTime;
		runningAnimation(animationClock);
		moving = true;
	}
	if (!moving)
		idleAnimation(animationClock);

	// Gravity
	if (position.y < 1080 - size.y && !isJumping)
	{
		velocity.y += speed * deltaTime;
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
	alignPlayerToHitBox();
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

void Player::getGridPos(Map& map)
{
	float cellSize = map.cellSize;

	gridPos.x = int (player->position.x) / cellSize;
	gridPos.y = int (player->position.y) / cellSize;
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

void Player::checkCollision(std::vector<std::vector<sf::RectangleShape>>& blocks)
{
	sf::FloatRect nextPos;

	bool tempGroundCheck = false;

	for (unsigned int y = gridPos.y - 2; y <= gridPos.y + 2; y++)
	{
		for (auto& block : blocks[y])
		{
			sf::FloatRect playerBounds = hitBox.getGlobalBounds();
			sf::FloatRect platformBounds = block.getGlobalBounds();

			nextPos = playerBounds;
			nextPos.position.x += velocity.x;
			nextPos.position.y += getVelocity().y;

			if (platformBounds.findIntersection(nextPos))
			{

				// Platform Top Collision
				if (playerBounds.position.y < platformBounds.position.y
					&& playerBounds.position.y + playerBounds.size.y < platformBounds.position.y + platformBounds.size.y
					&& playerBounds.position.x < platformBounds.position.x + platformBounds.size.x
					&& playerBounds.position.x + playerBounds.size.x > platformBounds.position.x)
				{
					velocity.y = 0.f;
					setPosition({ playerBounds.position.x, platformBounds.position.y - playerBounds.size.y });
					jumpClock.restart();
					isJumping = false;
					tempGroundCheck = true;
				}

				// Platform Bottom Collision
				else if (playerBounds.position.y > platformBounds.position.y
					&& playerBounds.position.y + playerBounds.size.y > platformBounds.position.y + platformBounds.size.y
					&& playerBounds.position.x < platformBounds.position.x + platformBounds.size.x
					&& playerBounds.position.x + playerBounds.size.x > platformBounds.position.x)
				{
					velocity.y = 0.f;
					setPosition({ playerBounds.position.x , platformBounds.position.y + platformBounds.size.y });
				}
				


				// Platform Left Collision
				if (playerBounds.position.x < platformBounds.position.x
					&& playerBounds.position.x + playerBounds.size.x < platformBounds.position.x + platformBounds.size.x
					&& playerBounds.position.y < platformBounds.position.y + platformBounds.size.y
					&& playerBounds.position.y + playerBounds.size.y > platformBounds.position.y)
				{
					velocity.x = 0.f;
					setPosition({ platformBounds.position.x - getSize().x, playerBounds.position.y });
				}

				// Platform Right Collision
				else if (playerBounds.position.x > platformBounds.position.x
					&& playerBounds.position.x + playerBounds.size.x > platformBounds.position.x + platformBounds.size.x
					&& playerBounds.position.y < platformBounds.position.y + platformBounds.size.y
					&& playerBounds.position.y + playerBounds.size.y > platformBounds.position.y)
				{
					velocity.x = 0.f;
					setPosition({ platformBounds.position.x + platformBounds.size.x ,playerBounds.position.y });
				}


			}
		}
	}

	onGround = tempGroundCheck;
}
