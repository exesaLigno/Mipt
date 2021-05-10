#include "Controllers/keyboard.hpp"
#include "Models/classic.hpp"
#include "Views/sfml.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>


int main()
{
// 	Controller controller;
// 	Model model;
// 	View view;
// 
// 	int event = controller.checkEvents();
// 
// 	model.processEvent(event);
// 	Block* game_view = model.getView();
// 
// 	view.draw(game_view);
// 
// 	return 0;
	
	setlocale(LC_ALL, "ru");
	
	sf::RenderWindow window(sf::VideoMode(800, 480), "Snake", sf::Style::Fullscreen);
	window.setFramerateLimit(60);
	sf::Vector2u size = window.getSize();
	
	Controller controller(&window);
	View view(&window);
	Model model(size.x/20, size.y/20);
		
	while (true)
	{
		int event = controller.checkEvents();
		
		if (event == EXIT)
			break;
		
		model.processEvent(event);
		Representation representation = model.getRepresentation();
		
		/* for (auto block : blocks)
			std::cout << "(" << block.position.x << ", " << block.position.y << ")";
		std::cout << std::endl; */
		
		view.draw(representation);
	}
	
	window.close();
	
	return 0;
}
