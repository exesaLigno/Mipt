#pragma once

#include "../Types/event.hpp"
#include "../Types/block.hpp"
#include "../Types/position.hpp"
#include <vector>


enum DIRECTIONS { UP = 1, LEFT, DOWN, RIGHT };


class Snake
{
	std::vector<Position> body;
	int length = 1;
	short direction = 0;
	
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
};


class Model
{

	bool game_running = false;
	
	bool food_existing = false;
	Position food_position;
	
	Snake snake;
	
	
  public:
	Model();
	~Model();
	void generateFood();
	int processEvent(int event);
	
};
