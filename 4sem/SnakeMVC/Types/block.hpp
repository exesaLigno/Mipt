#pragma once

#include "position.hpp"
#include <string>
#include <vector>


class Block
{
  public:
	int color = 0;
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
