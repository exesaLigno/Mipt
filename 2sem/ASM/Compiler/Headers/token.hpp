#pragma once


class Token
{
  private:
  	int type;
  	void* value;
  	
  public:
  	enum TYPES
  	{
  		name, number,
  		count_of_types
  	};
  	Token();
  	Token(char* text);
  	~Token();
};
