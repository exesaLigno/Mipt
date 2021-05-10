#include "classic.hpp"



Snake::Snake()
{
	generateDefaultSnake(5);
	direction = UP;
}

int Snake::generateDefaultSnake(int length)
{
	for (int counter = 0; counter < length; counter++)
		body.push_back(Position(10, 10 + counter));
	
	this -> length = length;
	
	return 0;
}


Snake::~Snake()
{}


Position Snake::getFutureHeadPosition()
{
	Position head = getHeadPosition();
	Position future_head;
	
	switch (direction)
	{
		case UP:
		{
			future_head = Position(head.x, head.y + 1);
			break;
		}
				
		case DOWN:
		{
			future_head = Position(head.x, head.y - 1);
			break;
		}
				
		case LEFT:
		{
			future_head = Position(head.x - 1, head.y);
			break;
		}
				
		case RIGHT:
		{
			future_head = Position(head.x + 1, head.y);
			break;
		}
				
		default:
			break;
	}
	
	return future_head;
}



int Snake::move()
{
	body.push_back(getFutureHeadPosition());
	
	if (body.size() > length)					// Preventing deleting tail if snake length increased
		body.erase(body.begin());
	
	rotation_blocked = false;
	
	return 0;
}

int Snake::feed()
{
	length += 1;
	return 0;
}

int Snake::changeDirection(int new_direction)
{
	if ((abs(new_direction - direction) % 2 == 1 or direction == NO_DIRECTION) and not rotation_blocked)
	{
		direction = new_direction;
		rotation_blocked = true;
	}
	
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
	Position head = getFutureHeadPosition();
	
	for (Position elem : body)
	{
		if (elem == head)
		{
			kill();
			break;
		}
	}
	
	return snake_alive;
}

void Snake::reset()
{
	body.clear();
	generateDefaultSnake(5);
		
	direction = UP;
	
	snake_alive = true;
}








Model::Model(int x, int y)
{
	window_size_x = x;
	window_size_y = y;
}


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
		{
			game_status = RUNNING;
			return 0;
		}
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
		
		if (clock.getElapsedTime().asMilliseconds() >= 150)
		{
			Position head = snake.getFutureHeadPosition();
			if (head.x < 0 or head.x >= window_size_x or head.y < 0 or head.y >= window_size_y)
				snake.kill();
			
			if (not snake.ok())
				game_status = ENDED;
			
			else
				snake.move();
			
			if (snake.getHeadPosition() == food_position)
			{
				snake.feed();
				generateFood();
			}
			
			clock.restart();
		}
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
	food_position = Position(rand() % window_size_x, rand() % window_size_y);
	food_existing = true;
}

void Model::resetGame()
{
	game_status = NOT_STARTED;
	snake.reset();
	food_existing = false;
}

std::vector<Block> Model::getBlocks()
{	
	std::vector<Block> blocks;
		
	for (Position block_position : snake.body)
	{
		blocks.push_back(Block(block_position, 1));
	}
	
	if (food_existing)
		blocks.push_back(Block(food_position, 2));
	
	return blocks;
}


Representation Model::getRepresentation()
{
	std::string center_text = "";
	
	if (game_status == NOT_STARTED)
		center_text = "Press W, A, S or D to start game";
	
	else if (game_status == ENDED)
		center_text = "You lose! Press R to restart";
	
	else if (game_status == PAUSED)
		center_text = "Pause. Press SPACE to continue";
	
	Representation representation(center_text, getBlocks());
	return representation;
}
