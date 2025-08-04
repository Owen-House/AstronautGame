#include "MainMenu.h"


MainMenu::MainMenu(Camera camera, sf::RenderWindow* window)
{
	sf::View cameraView = camera.GetView(window->getSize());
	font = Resources::fonts["ThaleahFat.ttf"];
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		menu[i] = new sf::Text(font, "", 150);
	}

	menu[0]->setFillColor(sf::Color::Cyan);
	menu[0]->setString("Play");
	sf::FloatRect bounds = menu[0]->getLocalBounds();
	menu[0]->setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });
	menu[0]->setPosition({ cameraView.getCenter().x, float(window->getSize().y) / (MAX_NUMBER_OF_ITEMS + 1) * 1});

	menu[1]->setFillColor(sf::Color::White);
	menu[1]->setString("Options");
	bounds = menu[1]->getLocalBounds();
	menu[1]->setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });
	menu[1]->setPosition({ cameraView.getCenter().x, float(window->getSize().y) / (MAX_NUMBER_OF_ITEMS + 1) * 2 });

	menu[2]->setFillColor(sf::Color::White);
	menu[2]->setString("Quit");
	bounds = menu[2]->getLocalBounds();
	menu[2]->setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });
	menu[2]->setPosition({ cameraView.getCenter().x, float(window->getSize().y) / (MAX_NUMBER_OF_ITEMS + 1) * 3 });

	selectedItemIndex = 0;
}

MainMenu::~MainMenu()
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		delete menu[i];
	}
}

void MainMenu::draw(sf::RenderWindow* window)
{
	for (int i = 0; i < 3; i++)
	{
		window->draw(*menu[i]);
	}
}
void MainMenu::moveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex]->setFillColor(sf::Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex]->setFillColor(sf::Color::Cyan);
	}
}

void MainMenu::moveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex]->setFillColor(sf::Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex]->setFillColor(sf::Color::Cyan);
	}
}