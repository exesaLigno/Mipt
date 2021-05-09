#pragma once


class Position
{
  public:
	int x = 0;
	int y = 0;
	
  public:
	Position();
	Position(int x, int y);
	~Position();
	
	bool operator==(Position other);
};
