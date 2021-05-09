#include "classic.hpp"



Snake::Snake()
{
	body.push_back(Position(5, 5));
}

Snake::~Snake()
{}

int Snake::move()
{
	Position head = getHeadPosition();
	
	switch (direction)
	{
		case UP:
		{
			body.push_back(Position(head.x, head.y + 1));
			break;
		}
		
		case DOWN:
		{
			body.push_back(Position(head.x, head.y - 1));
			break;
		}
		
		case LEFT:
		{
			body.push_back(Position(head.x - 1, head.y));
			break;
		}
		
		case RIGHT:
		{
			body.push_back(Position(head.x + 1, head.y));
			break;
		}
		
		default:
			break;
	}
	
	if (body.size() > length)					// Preventing deleting tail if snake length increased
		body.erase(body.begin());
	
	return 0;
}

int Snake::feed()
{
	length += 1;
	return 0;
}

int Snake::changeDirection(int new_direction)
{
	if (abs(new_direction - direction) % 2 == 1 or direction == NO_DIRECTION)
		direction = new_direction;
	
	return 0;
}

int Snake::kill()
{
	snake_alive = false;
	
	return 0;
}


Position Snake::getHeadPosition()
{
	return body.back();
}

bool Snake::ok()
{
	Position head = getHeadPosition();
	
	for (auto elem : body)
	{
		if (elem == head)
			return false;
	}
	
	return true;
}

void Snake::reset()
{
	body.clear();
	body.push_back(Position(5, 5));
	
	length = 1;
	
	direction = NO_DIRECTION;
	
	snake_alive = true;
}








Model::Model()
{}


Model::~Model()
{}


int Model::processEvent(int event)
{
	if (game_status == NOT_STARTED)
	{
		if (event == MOVE_UP or event == MOVE_DOWN or event == MOVE_LEFT or event == MOVE_RIGHT)
		{
			game_status = RUNNING;
			snake.changeDirection(event);
			generateFood();
		}
	}
	
	if (game_status == PAUSED)
	{
		if (event == PAUSE)
			game_status = RUNNING;
	}
	
	if (game_status == RUNNING)
	{
		if (event == MOVE_UP or event == MOVE_DOWN or event == MOVE_LEFT or event == MOVE_RIGHT)
			snake.changeDirection(event);
		
		else if (event == PAUSE)
		{
			game_status = PAUSED;
			return 0;
		}
		
		snake.move();
		
		if (snake.getHeadPosition() == food_position)
		{
			snake.feed();
			generateFood();
		}
		
		if (not snake.ok())
			game_status = ENDED;
	}
	
	if (game_status == ENDED)
	{
		if (event == RESTART)
			resetGame();
	}
	
	return 0;
}

void Model::generateFood()
{
	food_position = Position(2, 3);
}

void Model::resetGame()
{
	game_status = NOT_STARTED;
	snake.reset();
}

Block* Model::getView()
{
	return nullptr;
}
