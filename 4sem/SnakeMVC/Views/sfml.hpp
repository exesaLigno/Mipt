#pragma once
#include <SFML/Graphics.hpp>
#include "../Types/block.hpp"
#include <vector>
#include <iostream>


class View
{
	sf::RenderWindow* window = nullptr;
	sf::Font* font;
	
  public:
	View(sf::RenderWindow* window);
	~View();
	int draw(Representation game_view);
};
