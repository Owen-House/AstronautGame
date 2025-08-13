#include "Enemy.h"

sf::IntRect animationRect = sf::IntRect({0, 0}, {16, 16});


Enemy::Enemy(sf::Vector2f hitBoxSize, sf::Texture texture, float health, float damage, sf::Vector2f position, sf::Vector2f size, int yGridPos, float speed)
	: texture(texture), health(health), damage(damage), position(position), size(size), yGridPos(yGridPos), speed(speed)
{
    hitBox.setPosition(position);
    hitBox.setSize(hitBoxSize);
    hitBox.setFillColor(sf::Color::Transparent);
    hitBox.setOutlineColor(sf::Color::White);
    hitBox.setOutlineThickness(-.4f);
    hitBox.setScale(sf::Vector2f({ size.x / animationRect.size.x, size.y / animationRect.size.y }));
}

void Enemy::Draw(Renderer& renderer)
{
    if (isFacingLeft)
    {
        faceLeft();
        renderer.Draw(texture, position, size, animationRect);
    }
    else
    {
        faceRight();
        renderer.Draw(texture, position, { -size.x, size.y }, animationRect);
    }
	
}

void Enemy::CheckCollision(const std::vector< std::vector<sf::RectangleShape>>& blocks, float deltaTime)
{
    sf::FloatRect nextPos;
    for (auto& block : blocks[yGridPos])
    {
        sf::FloatRect enemyBounds = hitBox.getGlobalBounds();
        sf::FloatRect platformBounds = block.getGlobalBounds();

        nextPos = enemyBounds;
        nextPos.position.x += speed * deltaTime;



        if (platformBounds.findIntersection(nextPos))
        {
            // Platform Left Collision
            if (enemyBounds.position.x < platformBounds.position.x
                && enemyBounds.position.x + enemyBounds.size.x < platformBounds.position.x + platformBounds.size.x
                && enemyBounds.position.y < platformBounds.position.y + platformBounds.size.y
                && enemyBounds.position.y + enemyBounds.size.y > platformBounds.position.y)
            {
                speed = -speed;
                position = { platformBounds.position.x - size.x, enemyBounds.position.y };
                isFacingLeft = false;
                alignToHitBox();
            }

            // Platform Right Collision
            else if (enemyBounds.position.x > platformBounds.position.x
                && enemyBounds.position.x + enemyBounds.size.x > platformBounds.position.x + platformBounds.size.x
                && enemyBounds.position.y < platformBounds.position.y + platformBounds.size.y
                && enemyBounds.position.y + enemyBounds.size.y > platformBounds.position.y)
            {
                speed = -speed;
                position = { platformBounds.position.x + platformBounds.size.x ,enemyBounds.position.y };
                isFacingLeft = true;
                alignToHitBox();
            }


        }
    }
}

bool Enemy::checkPlayerCollision(Player* player)
{
    if (player->getHitbox().getGlobalBounds().findIntersection(hitBox.getGlobalBounds()))
    {
        std::cout << "GAME OVER" << std::endl;
        resetGame();
        return true;
    }
    return false;
}

void Enemy::faceRight()
{
    offset = { -12, 6 };
    alignToHitBox();
}

void Enemy::faceLeft()
{
    offset = { 3, 6 };
    alignToHitBox();
}

void Enemy::move(float deltaTime)
{
    hitBox.move({ speed * deltaTime, 0});
}

void Enemy::alignToHitBox()
{
    position = { hitBox.getPosition().x - offset.x * scale, hitBox.getPosition().y - offset.y * scale };
}