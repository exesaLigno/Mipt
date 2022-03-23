#pragma once

#include "../Types/block.hpp"
#include "../Types/position.hpp"
#include "../Types/event.hpp"
#include <vector>
#include <math.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>




enum DIRECTIONS { NO_DIRECTION, UP, LEFT, DOWN, RIGHT };
enum GAME_STATUS { NOT_STARTED, RUNNING, ENDED, PAUSED };
enum AI_LEVELS { EASY, MEDIUM, HARD, IMPOSSIBLE };


class Snake
{
  public:
	std::vector<Position> body;
	int length = 1;
	short direction = NO_DIRECTION;
	bool rotation_blocked = false;
	
	bool snake_alive = true;
	
	int generating_area_x = 0;
	int generating_area_y = 0;
	
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
	bool contains(Position other_head);
	
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
	Snake enemy;
	
	sf::Clock clock;
	
	int score = 0;
	int enemy_score = 0;
	
	int ai_level = HARD;
	
	
  public:
	Model(int x, int y);
	~Model();
	
	void generateFood();
	int processEvent(Event event);
	void resetGame();
	
	void ai();
	bool aiCheck();
	int aiMakeTrap();
	
	std::vector<Block> getBlocks();
	Representation getRepresentation();
};


int new_random_direction(int old_direction);