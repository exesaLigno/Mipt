#include "classic.hpp"
#include <math.h>



Snake::Snake()
{
	(this -> body).push_back(Position(5, 5));
}

Snake::~Snake()
{}

int Snake::move()
{
	Position head = (this -> body).back();
	
	switch (this -> direction)
	{
		case UP:
		{
			(this -> body).push_back(Position(head.x, head.y + 1));
			break;
		}
		
		case DOWN:
		{
			(this -> body).push_back(Position(head.x, head.y - 1));
			break;
		}
		
		case LEFT:
		{
			(this -> body).push_back(Position(head.x - 1, head.y));
			break;
		}
		
		case RIGHT:
		{
			(this -> body).push_back(Position(head.x + 1, head.y));
			break;
		}
		
		default:
			break;
	}
	
	if (body.size() > this -> length)					// Preventing deleting tail if snake length increased
		(this -> body).erase((this -> body).begin());
	
	return 0;
}

int Snake::feed()
{
	this -> length += 1;
	return 0;
}

int Snake::changeDirection(int new_direction)
{
	if (abs(direction - this -> direction) % 2 == 1 or this -> direction == 0)
		this -> direction = new_direction;
	
	return 0;
}

int Snake::kill()
{
	this -> snake_alive = false;
	
	return 0;
}


Position Snake::getHeadPosition()
{
	return (this -> body).back();
}

bool Snake::ok()
{
	Position head = this -> getHeadPosition();
	
	for (auto elem : body)
	{
		if (elem == head)
			return false;
	}
	
	return true;
}







Model::Model()
{}


Model::~Model()
{}


int Model::processEvent(int event)
{
	if (not food_existing)
		generateFood();
	
	if (event != 0)							// Processing command
	{
		snake.changeDirection(event);
		game_running = true;
	}
	
	if (snake.getHeadPosition() == food_position)	// Eating food
	{
		snake.feed();
		food_existing = false;
	}
	
	if (snake.ok() and game_running)	// Moving snake
		snake.move();
	
	if (not food_existing)
		generateFood();
	
	return 0;
}


