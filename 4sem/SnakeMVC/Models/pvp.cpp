#include "pvp.hpp"



Snake::Snake()
{
	generateDefaultSnake(5);
}

int Snake::generateDefaultSnake(int length)
{
	direction = rand() % 4 + 1;
	
	int d1 = 0, d2 = 0;
	
	switch (direction)
	{
		case UP:
		{
			d1 = 0;
			d2 = 1;
			break;
		}
		
		case DOWN:
		{
			d1 = 0;
			d2 = -1;
			break;
		}
		
		case RIGHT:
		{
			d1 = 1;
			d2 = 0;
			break;
		}
				
		case LEFT:
		{
			d1 = -1;
			d2 = 0;
			break;
		}
	}
	
	int x_init = rand() % (generating_area_x - 8) + 4;
	int y_init = rand() % (generating_area_y - 8) + 4;
	
	for (int counter = 0; counter < length; counter++)
		body.push_back(Position(x_init + counter * d1, y_init + counter * d2));
	
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
			future_head = head;
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
			return false;
			break;
		}
	}
		
	return snake_alive;
}

void Snake::reset()
{
	body.clear();
	generateDefaultSnake(5);
	
	snake_alive = true;
}

bool Snake::contains(Position other_head)
{
	for (Position elem : body)
	{
		if (elem == other_head)
			return true;
	}
	
	return false;
}









Model::Model(int x, int y)
{
	window_size_x = x;
	window_size_y = y;
	
	snake.generating_area_x = x;
	snake.generating_area_y = y;
	
	enemy.generating_area_x = x;
	enemy.generating_area_y = y;
	
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	
	srand((time_t)ts.tv_nsec);
}


Model::~Model()
{}


int Model::processEvent(Event event)
{	
	if (game_status == NOT_STARTED)
	{
		if (event == MOVE_UP or event == MOVE_DOWN or event == MOVE_LEFT or event == MOVE_RIGHT)
		{
			game_status = RUNNING;
			snake.changeDirection(event.event_code);
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
			snake.changeDirection(event.event_code);
		
		else if (event == PAUSE)
		{
			game_status = PAUSED;
			return 0;
		}
		
		if (clock.getElapsedTime().asMilliseconds() >= 150)
		{
			Position head = snake.getFutureHeadPosition();
			if (head.x < 0 or head.x >= window_size_x or head.y < 0 or head.y >= window_size_y or enemy.contains(head))
				snake.kill();
			
			if (not snake.ok())
				game_status = ENDED;
			
			else
			{
				snake.move();
				ai();
			}
			
			if (snake.getHeadPosition() == food_position)
			{
				snake.feed();
				score += 5;
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
	
	if (event == RESIZE)
	{
		this -> window_size_x = event.width / 20;
		this -> window_size_y = event.height / 20;
		
		snake.generating_area_x = window_size_x;
		snake.generating_area_y = window_size_y;
		
		enemy.generating_area_x = window_size_x;
		enemy.generating_area_y = window_size_y;
	}
	
	return 0;
}



void Model::ai()
{	
	if (aiCheck())
	{		
		enemy.direction = new_random_direction(enemy.direction); // enemy.direction % 4 + 1;
		if (aiCheck())
		{
			enemy.direction = (enemy.direction + 1) % 4 + 1;
		}
		
		if (aiCheck())
		{
			enemy.kill();
			enemy.reset();
			enemy_score = 0;
		}
	}
	
	else 
	{		
		int old_direction = enemy.direction;
		int vertical_delta = food_position.y - enemy.getHeadPosition().y;
		int horizontal_delta = food_position.x - enemy.getHeadPosition().x;
		
		if (vertical_delta == 0 and (enemy.direction == UP or enemy.direction == DOWN) and ai_level >= EASY)
		{
			if (food_position.x > enemy.getHeadPosition().x)
				enemy.changeDirection(RIGHT);
			
			else if (food_position.x < enemy.getHeadPosition().x)
				enemy.changeDirection(LEFT);
		}
		
		else if (horizontal_delta == 0 and (enemy.direction == LEFT or enemy.direction == RIGHT) and ai_level >= EASY)
		{
			if (food_position.y > enemy.getHeadPosition().y)
				enemy.changeDirection(UP);
			
			else if (food_position.y < enemy.getHeadPosition().y)
				enemy.changeDirection(DOWN);
		}
		
		else if ((enemy.direction == UP or enemy.direction == DOWN) and ai_level >= MEDIUM)
		{
			if (food_position.x > enemy.getHeadPosition().x)
				enemy.changeDirection(RIGHT);
			
			else if (food_position.x < enemy.getHeadPosition().x)
				enemy.changeDirection(LEFT);
		}
		
		else if (enemy.direction == LEFT and food_position.x > enemy.getHeadPosition().x and ai_level >= HARD)
			enemy.changeDirection(rand() % 2 ? UP : DOWN);
		
		else if (enemy.direction == RIGHT and food_position.x < enemy.getHeadPosition().x and ai_level >= HARD)
			enemy.changeDirection(rand() % 2 ? UP : DOWN);
		
		else if (ai_level >= IMPOSSIBLE)
			aiMakeTrap();
		
		if (aiCheck())
		{
			enemy.direction = old_direction;
		}
	}
	
	if (enemy.getFutureHeadPosition() == food_position)
	{
		enemy.feed();
		generateFood();
		enemy_score += 5;
	}
	
	enemy.move();
}

bool Model::aiCheck()
{
	Position future_head = enemy.getFutureHeadPosition();
	
	return snake.contains(future_head) or future_head.x < 0 or future_head.y < 0 or future_head.x >= window_size_x or future_head.y >= window_size_y or not enemy.ok();
}


int Model::aiMakeTrap()
{
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
	enemy.reset();
	
	score = 0;
	enemy_score = 0;
	
	food_existing = false;
}

std::vector<Block> Model::getBlocks()
{
	std::vector<Block> blocks;
		
	for (Position block_position : snake.body)
		blocks.push_back(Block(block_position, WHITE_SNAKE_BODY));
	
	blocks.back().style = WHITE_SNAKE_HEAD;
	
	for (Position block_position : enemy.body)
		blocks.push_back(Block(block_position, RED_SNAKE_BODY));
	
	blocks.back().style = RED_SNAKE_HEAD;
	
	if (food_existing)
		blocks.push_back(Block(food_position, GREEN_FOOD));
	
	return blocks;
}


Representation Model::getRepresentation()
{
	std::string center_text = "";
	
	if (game_status == NOT_STARTED)
		center_text = "Press W, A, S or D to start game. You can also use arrows.";
	
	else if (game_status == ENDED)
		center_text = "Game over! Press R to restart.\n\n\t\t\t\t\tScore: " + std::to_string(score);
	
	else if (game_status == PAUSED)
		center_text = "Pause. Press SPACE to continue.\n\n\t\t\t\t\tScore: " + std::to_string(score);
	
	Representation representation(center_text, getBlocks());
	return representation;
}




int new_random_direction(int old_direction)
{
	int randomizer = rand() % 2;
	
	if (old_direction == LEFT or old_direction == RIGHT)
		return UP * randomizer + DOWN * !randomizer;
	
	else
		return LEFT * randomizer + RIGHT * !randomizer;
}
