#pragma once

#include "game/Board.h"
#include <string>

using std::string;
using std::vector;

class Board;

class Figure
{
public:
	Figure(Board* board, unsigned int x, unsigned int y, bool figureColor);
	//Figure(Board* board, string pos);
	~Figure();
	
	//all theoretical moves
	virtual bool canMove(string to);
	virtual bool canMove(unsigned int x, unsigned int y);

	//checks if cell is empty or is occupied by figure with other color than this
	bool canReach(unsigned int x, unsigned y);

	bool isAllowedMove(unsigned int x, unsigned int y);

	string toString();
	const static bool WHITE = true;
	const static bool BLACK = false;
	bool figureColor;
	unsigned int x;
	unsigned int y;
	Board* board;
};