#include "Controllers/keyboard.hpp"
#include "Models/pvp.hpp"
#include "Views/sfml.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Types/event.hpp"


int main()
{
	setlocale(LC_ALL, "ru");
	
	sf::RenderWindow window(sf::VideoMode(800, 480), "Snake PVP");
	window.setFramerateLimit(60);
	sf::Vector2u size = window.getSize();
	
	Controller controller(&window);
	View view(&window);
	Model model(size.x/20, size.y/20);
	
	while (true)
	{
		Event event = controller.checkEvents();
		
		if (event == EXIT)
			break;
		
		model.processEvent(event);
		Representation representation = model.getRepresentation();
		
		view.draw(representation);
	}
	
	window.close();
	
	return 0;
}
