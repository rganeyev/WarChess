#pragma once

#include "game/Board.h"
#include <string>

class Board;

class Figure
{
public:
	Figure(Board* board, unsigned int x, unsigned int y, bool figureColor);
	//Figure(Board* board, string pos);
	~Figure();
	
	//all theoretical moves
	virtual bool canMove(const char* to);
	virtual bool canMove(unsigned int x, unsigned int y);

	//checks if cell is empty or is occupied by figure with other color than this
	bool canReach(unsigned int x, unsigned y);

	//checks if we can move with that figure without check
	bool isAllowedMove(unsigned int x, unsigned int y);

	//moves figure to cell
	void move(unsigned int x, unsigned int y);

	const static bool WHITE = true;
	const static bool BLACK = false;
	bool figureColor;
	unsigned int x;
	unsigned int y;
	Board* board;
};