#pragma once

#include "position.hpp"


class Block
{
  public:
	int color = 0;
	Position position;
	
  public:
	Block();
	~Block();
};
