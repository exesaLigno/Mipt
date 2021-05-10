#include "keyboard.hpp"


Controller::Controller(sf::RenderWindow* window_lnk)
{
	window = window_lnk;
}


Controller::~Controller()
{}


int Controller::checkEvents()
{
	int command = 0;
	
	sf::Event event;
	window -> pollEvent(event);
	
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Up)
			command = 1;
		
		else if (event.key.code == sf::Keyboard::Down)
			command = 3;
		
		else if (event.key.code == sf::Keyboard::Left)
			command = 2;
		
		else if (event.key.code == sf::Keyboard::Right)
			command = 4;
		
		else if (event.key.code == sf::Keyboard::R)
			command = 5;
		
		else if (event.key.code == sf::Keyboard::Space)
			command = 6;
		
		else if (event.key.code == sf::Keyboard::Escape)
			command = 7;
		
		else if (event.key.code == sf::Keyboard::LShift or event.key.code == sf::Keyboard::RShift)
			command = 8;
	}
	
	else if (event.type == sf::Event::Closed)
	{
		command = 7;
	}
		
	return command;
}


