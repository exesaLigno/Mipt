#include "keyboard.hpp"


Controller::Controller(sf::RenderWindow* window_lnk)
{
	window = window_lnk;
}


Controller::~Controller()
{}


Event Controller::checkEvents()
{
	Event command = NO_COMMAND;
	
	sf::Event event;
	window -> pollEvent(event);
	
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Up or event.key.code == sf::Keyboard::W)
			command = MOVE_UP;
		
		else if (event.key.code == sf::Keyboard::Down or event.key.code == sf::Keyboard::S)
			command = MOVE_DOWN;
		
		else if (event.key.code == sf::Keyboard::Left or event.key.code == sf::Keyboard::A)
			command = MOVE_LEFT;
		
		else if (event.key.code == sf::Keyboard::Right or event.key.code == sf::Keyboard::D)
			command = MOVE_RIGHT;
		
		else if (event.key.code == sf::Keyboard::R)
			command = RESTART;
		
		else if (event.key.code == sf::Keyboard::Space)
			command = PAUSE;
		
		else if (event.key.code == sf::Keyboard::Escape)
			command = EXIT;
		
		else if (event.key.code == sf::Keyboard::LShift or event.key.code == sf::Keyboard::RShift)
			command = ACCELERATE;
	}
	
	else if (event.type == sf::Event::Closed and event.key.code != 0)	// It is crutch to compare key code with 0
	{
		command = EXIT;
	}
	
	else if (event.type == sf::Event::Resized)
	{
		command = RESIZE;
		command.height = event.size.height;
		command.width = event.size.width;
				
		window -> setView(sf::View(sf::Vector2f(event.size.width / 2.0, event.size.height / 2.0), sf::Vector2f(event.size.width, event.size.height)));
	}
		
	return command;
}


