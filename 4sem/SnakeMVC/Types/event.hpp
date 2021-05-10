#pragma once

enum EVENTS { NO_COMMAND, MOVE_UP, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, RESTART, PAUSE, EXIT, ACCELERATE, RESIZE };

class Event
{
  public:
	int event_code = NO_COMMAND;
	int height = 0;
	int width = 0;
	
	Event(int event_type);
	
	bool operator==(int command_type);
};
