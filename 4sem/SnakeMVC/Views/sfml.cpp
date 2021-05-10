#include "sfml.hpp"


View::View(sf::RenderWindow* window)
{
	this -> window = window;
	this -> font = new sf::Font();
	if (! this -> font -> loadFromFile("Views/20339.ttf"))
		std::cout << "error" << std::endl;
	
	clock.restart();
}


View::~View()
{}


int View::draw(Representation game_view)
{
	if (clock.getElapsedTime().asMilliseconds() >= 15)
	{
		frame_number++;
		clock.restart();
		
		window -> clear();
	
		sf::Vector2u size = window -> getSize();
			
		for (Block block : game_view.blocks)
		{
			if (block.style == WHITE_SNAKE_BODY)
			{
				sf::RectangleShape square(sf::Vector2f(18.f, 18.f));
				square.setPosition(block.position.x * 20 + 1, size.y - (block.position.y + 1) * 20 + 1);
				square.setOutlineThickness(1.f);
				square.setOutlineColor(sf::Color::Black);
				square.setFillColor(sf::Color::White);
				window -> draw(square);
			}
			
			else if (block.style == WHITE_SNAKE_HEAD)
			{
				sf::RectangleShape square(sf::Vector2f(18.f, 18.f));
				square.setPosition(block.position.x * 20 + 1, size.y - (block.position.y + 1) * 20 + 1);
				square.setOutlineThickness(1.f);
				square.setOutlineColor(sf::Color::Black);
				square.setFillColor(sf::Color::White);
				window -> draw(square);
				
				sf::RectangleShape eye(sf::Vector2f(8.f, 8.f));
				eye.setPosition(block.position.x * 20 + 6, size.y - (block.position.y + 1) * 20 + 6);
				eye.setOutlineThickness(1.f);
				eye.setOutlineColor(sf::Color::Black);
				eye.setFillColor(sf::Color::Black);
				window -> draw(eye);
			}
			
			else if (block.style == RED_SNAKE_BODY)
			{
				sf::RectangleShape square(sf::Vector2f(18.f, 18.f));
				square.setPosition(block.position.x * 20 + 1, size.y - (block.position.y + 1) * 20 + 1);
				square.setOutlineThickness(1.f);
				square.setOutlineColor(sf::Color::Black);
				square.setFillColor(sf::Color::Red);
				window -> draw(square);
			}
			
			else if (block.style == RED_SNAKE_HEAD)
			{
				sf::RectangleShape square(sf::Vector2f(18.f, 18.f));
				square.setPosition(block.position.x * 20 + 1, size.y - (block.position.y + 1) * 20 + 1);
				square.setOutlineThickness(1.f);
				square.setOutlineColor(sf::Color::Black);
				square.setFillColor(sf::Color::Red);
				window -> draw(square);
				
				sf::RectangleShape eye(sf::Vector2f(8.f, 8.f));
				eye.setPosition(block.position.x * 20 + 6, size.y - (block.position.y + 1) * 20 + 6);
				eye.setOutlineThickness(1.f);
				eye.setOutlineColor(sf::Color::Black);
				eye.setFillColor(sf::Color::Black);
				window -> draw(eye);
			}
			
			else if (block.style == GREEN_FOOD)
			{
				float radius = 9.f;
				
				if ((frame_number / 10) % 5 == 1)
					radius = 8.f;
				else if ((frame_number / 10) % 5 == 2)
					radius = 7.f;
				else if ((frame_number / 10) % 5 == 3)
					radius = 8.f;
					
				sf::CircleShape circle(radius);
				circle.setPosition(block.position.x * 20 + (10 - radius), size.y - (block.position.y + 1) * 20 + (10 - radius));
				circle.setOutlineThickness(1.f);
				circle.setOutlineColor(sf::Color::Black);
				circle.setFillColor(sf::Color::Green);
				window -> draw(circle);
			}
		}
		
		if (game_view.center_text.length() != 0)
		{
			sf::RectangleShape background(sf::Vector2f(size.x, size.y));
			background.setPosition(0, 0);
			background.setFillColor(sf::Color(0, 0, 0, 200));
			window -> draw(background);
			
			sf::Text text;
			text.setString(game_view.center_text);
			text.setCharacterSize(24);
			text.setFont(*(this -> font));
			//text.setColor(sf::Color::Red);
			text.setPosition(size.x / 2 - text.getGlobalBounds().width / 2, size.y / 2 - text.getGlobalBounds().height / 2);
			
			window -> draw(text);		
		}
		
		
		window -> display();
	}
		
	return 0;
}
