#include "Controllers/keyboard.hpp"
#include "Models/classic.hpp"
#include "Views/sfml.hpp"


int main()
{
	Controller controller;
	Model model;
	View view;
	
	int event = controller.checkEvents();
	
	model.processEvent(event);
	Block* game_view = model.getView();
	
	view.draw(game_view);
	
	return 0;
}
