#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Resources.h"
#include "Renderer.h"
#include <vector>

#define MAX_NUMBER_OF_ITEMS 4

class Menu
{

public:
	Menu(Camera camera, sf::RenderWindow* window);
	~Menu();


	void draw(sf::RenderWindow* window, Renderer& renderer);
	void moveUp();
	void moveDown();
	const int getPressedItem() { return selectedItemIndex; }
	void updatePosition(Camera camera, sf::RenderWindow* window);
	void updateLevel(Camera camera, sf::RenderWindow* window, int currLevel);
	
private:
	float xPosition;
	int selectedItemIndex;
	sf::Font font;
	sf::Text* menu[MAX_NUMBER_OF_ITEMS];
	sf::Color selectedColor = sf::Color::Cyan;
	sf::Color unselectedColor = sf::Color::White;

};

