#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Types/event.hpp"




class Controller
{
	sf::RenderWindow* window = nullptr;
	
  public:
	Controller(sf::RenderWindow* window_lnk);
	~Controller();
	Event checkEvents();
};
