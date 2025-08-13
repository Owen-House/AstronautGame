#include "Camera.h"
#include "Map.h"

Camera::Camera(float zoomLevel)
	: zoomLevel(zoomLevel)
{
}

sf::View Camera::GetView(sf::Vector2u windowSize)
{
	float aspect = (float)windowSize.x / (float)windowSize.y;
	sf::Vector2f size;
	if (aspect < 1.0f)
		size = sf::Vector2f(zoomLevel, zoomLevel / aspect);
	else
		size = sf::Vector2f(zoomLevel * aspect, zoomLevel);

	return sf::View(position, size);
}

void Camera::moveWithPlayer(Player* player, Map& map, sf::Vector2u windowSize)
{
	sf::View cameraView = GetView(windowSize);

	if (position.x > player->getPosition().x + 300 && position.x > (cameraView.getSize().x / 2))
	{
		// Move Camera Left
		position.x -= std::abs(player->getVelocity().x);
	}
	else if (position.x <= (cameraView.getSize().x / 2))
	{
		position.x = (cameraView.getSize().x / 2);
	}
	if ((position.x < player->getPosition().x - 300) && position.x < (map.grid.size()) * 54 - (cameraView.getSize().x / 2) - 2)
	{
		// Move Camera Right
		position.x += std::abs(player->getVelocity().x);
	}
	else if (position.x >= (map.grid.size()) * 54 - (cameraView.getSize().x / 2) - 2)
	{
		position.x = (map.grid.size()) * 54 - (cameraView.getSize().x / 2) - 2;
	}

}