#pragma once

#include "position.hpp"
#include <string>
#include <vector>


enum STYLES { NO_STYLE, WHITE_SNAKE_BODY, GREEN_FOOD, RED_SNAKE_BODY, GOLD_FOOD, WHITE_SNAKE_HEAD, RED_SNAKE_HEAD };



class Block
{
  public:
	int style = 0;
	Position position;
	
  public:
	Block();
	Block(Position pos, int col);
	~Block();
};


class Representation
{
public:
	std::string center_text;
	std::vector<Block> blocks;
	
	Representation(std::string text, std::vector<Block> blocks);
	~Representation();
};
