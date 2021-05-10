#include "position.hpp"


Position::Position()
{}

Position::Position(int x, int y)
{
	this -> x = x;
	this -> y = y;
}

Position::~Position()
{}

bool Position::operator==(Position& other)
{
	if (x == other.x and y == other.y)
		return true;
	
	else
		return false;
}
