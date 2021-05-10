#include "block.hpp"


Block::Block()
{}

Block::Block(Position pos, int col)
{
	position = pos;
	color = col;
}


Block::~Block()
{}



Representation::Representation(std::string text, std::vector<Block> blocks)
{
	this -> center_text = text;
	this -> blocks = blocks;
}


Representation::~Representation()
{}
