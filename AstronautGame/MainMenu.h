#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "Renderer.h"
#include "Resources.h"
#include "Camera.h"
#include "Enemy.h"
#include "MainMenu.h"
#include <vector>

#define MAX_NUMBER_OF_ITEMS 3

class MainMenu
{

public:
	MainMenu(float width, float height);
	~MainMenu();

	void draw(sf::RenderWindow* window);
	void moveUp();
	void moveDown();
	int getPressedItem() { return selectedItemIndex; }

	bool inMenu;
private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text* menu[MAX_NUMBER_OF_ITEMS];
};

