#pragma once


class Position
{
  public:
	int x = -1;
	int y = -1;
	
  public:
	Position();
	Position(int x, int y);
	~Position();
	
	bool operator==(Position& other);
};