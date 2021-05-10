#pragma once

#include "../Types/block.hpp"
#include "../Types/position.hpp"
#include <vector>
#include <math.h>
#include <iostream>
#include <SFML/Graphics.hpp>




enum DIRECTIONS { NO_DIRECTION, UP, LEFT, DOWN, RIGHT };
enum EVENTS { NO_COMMAND, MOVE_UP, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, RESTART, PAUSE, EXIT, ACCELERATE };
enum GAME_STATUS { NOT_STARTED, RUNNING, ENDED, PAUSED };


class Snake
{
  public:
	std::vector<Position> body;
	int length = 1;
	short direction = NO_DIRECTION;
	bool rotation_blocked = false;
	
	bool snake_alive = true;
	
  public:
	Snake();
	int generateDefaultSnake(int length);
	~Snake();
	
	Position getFutureHeadPosition();
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
	int window_size_x = 0;
	int window_size_y = 0;

	int game_status = NOT_STARTED;
	
	Position food_position;
	bool food_existing = false;
	
	Snake snake;
	
	sf::Clock clock;
	
	int score = 0;
	
	
  public:
	Model(int x, int y);
	~Model();
	void generateFood();
	int processEvent(int event);
	void resetGame();
	std::vector<Block> getBlocks();
	Representation getRepresentation();
};
