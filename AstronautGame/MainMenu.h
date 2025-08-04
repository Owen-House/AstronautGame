#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Resources.h"
#include "Renderer.h"
#include <vector>

#define MAX_NUMBER_OF_ITEMS 3

class MainMenu
{

public:
	MainMenu(Camera camera, sf::RenderWindow* window);
	~MainMenu();


	void draw(sf::RenderWindow* window, Renderer& renderer);
	void moveUp();
	void moveDown();
	const int getPressedItem() { return selectedItemIndex; }

private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text* menu[MAX_NUMBER_OF_ITEMS];
};

