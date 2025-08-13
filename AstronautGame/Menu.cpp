#include "Menu.h"


Menu::Menu(Camera camera, sf::RenderWindow* window)
{
	sf::View cameraView = camera.GetView(window->getSize());
	font = Resources::fonts["ThaleahFat.ttf"];
	sf::FloatRect bounds;
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		menu[i] = new sf::Text(font, "", 150);
		menu[i]->setFillColor(unselectedColor);
		menu[i]->setOutlineColor(sf::Color::Blue);
		menu[i]->setOutlineThickness(2);
	}

	menu[0]->setString("Astronaut");
	menu[0]->setFillColor(sf::Color::Black);
	menu[0]->setOutlineColor(sf::Color::White);
	menu[0]->setOutlineThickness(4);
	menu[0]->setCharacterSize(300);

	menu[1]->setFillColor(selectedColor);
	menu[1]->setString("Play");

	menu[2]->setString("Current Level: ");

	menu[3]->setString("Quit");

	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		bounds = menu[i]->getLocalBounds();
		menu[i]->setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });
		menu[i]->setPosition({ xPosition, float(window->getSize().y) / (MAX_NUMBER_OF_ITEMS + 1) * (i + 1)});
	}

	// Shift Title up
	menu[0]->setPosition({ xPosition, menu[0]->getPosition().y - 100});
	
	selectedItemIndex = 1;
	xPosition = cameraView.getCenter().x;
}

Menu::~Menu()
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		delete menu[i];
	}
}

void Menu::draw(sf::RenderWindow* window, Renderer& renderer)
{
	renderer.Draw(Resources::textures["stars.png"], { xPosition - window->getSize().x / 2, 0}, sf::Vector2f(window->getSize()),
		sf::IntRect({ 0, 0 }, { int(Resources::textures["stars.png"].getSize().x), int(Resources::textures["stars.png"].getSize().y) }));
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		window->draw(*menu[i]);
	}
}

void Menu::moveUp()
{
	if (selectedItemIndex - 1 >= 1)
	{
		menu[selectedItemIndex]->setFillColor(unselectedColor);
		selectedItemIndex--;
		menu[selectedItemIndex]->setFillColor(selectedColor);
	}
}

void Menu::moveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex]->setFillColor(unselectedColor);
		selectedItemIndex++;
		menu[selectedItemIndex]->setFillColor(selectedColor);
	}
}

void Menu::updatePosition(Camera camera, sf::RenderWindow* window)
{
	xPosition = camera.GetView(window->getSize()).getCenter().x;
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		menu[i]->setPosition({ xPosition, menu[i]->getPosition().y });
	}
}

void Menu::updateLevel(Camera camera, sf::RenderWindow* window, int currLevel)
{
	menu[2]->setString("Current Level: " + std::to_string(currLevel));
	sf::FloatRect bounds = menu[2]->getLocalBounds();
	menu[2]->setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });
	menu[2]->setPosition({ xPosition, float(window->getSize().y) / (MAX_NUMBER_OF_ITEMS + 1) * 3 });
}