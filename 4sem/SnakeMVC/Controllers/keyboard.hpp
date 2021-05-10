#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>




class Controller
{
	sf::RenderWindow* window = nullptr;
	
  public:
	Controller(sf::RenderWindow* window_lnk);
	~Controller();
	int checkEvents();
};
