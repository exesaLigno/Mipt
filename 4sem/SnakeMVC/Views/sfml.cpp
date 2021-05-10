#include "sfml.hpp"


View::View(sf::RenderWindow* window)
{
	this -> window = window;
	this -> font = new sf::Font();
	if (! this -> font -> loadFromFile("Views/20339.ttf"))
		std::cout << "error" << std::endl;
}


View::~View()
{}


int View::draw(Representation game_view)
{
	window -> clear();
	
	sf::Vector2u size = window -> getSize();
	
	for (Block block : game_view.blocks)
	{
		sf::RectangleShape square(sf::Vector2f(18.f, 18.f));
		square.setPosition(block.position.x * 20 + 1, size.y - (block.position.y + 1) * 20 + 1);
		square.setOutlineThickness(1.f);
		square.setOutlineColor(sf::Color::Black);
		square.setFillColor(block.color == 1 ? sf::Color::White : sf::Color::Green);
		window -> draw(square);
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
		
	return 0;
}
