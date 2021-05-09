#pragma once

#include "../Types/event.hpp"
#include "../Types/block.hpp"
#include "../Types/position.hpp"
#include <vector>
#include <math.h>


enum DIRECTIONS { NO_DIRECTION, UP, LEFT, DOWN, RIGHT };
enum GAME_STATUS { NOT_STARTED, RUNNING, ENDED, PAUSED };


class Snake
{
	std::vector<Position> body;
	int length = 1;
	short direction = NO_DIRECTION;
	
	bool snake_alive = true;
	
  public:
	Snake();
	~Snake();
	
	int move();
	int changeDirection(int new_direction);
	int feed();
	int kill();
	
	bool ok();
	Position getHeadPosition();
	
	void reset();
};


class Model
{

	int game_status = NOT_STARTED;
	
	bool food_existing = false;
	Position food_position;
	
	Snake snake;
	
	
  public:
	Model();
	~Model();
	void generateFood();
	int processEvent(int event);
	
};
