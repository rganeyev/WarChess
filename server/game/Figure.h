#pragma once

#include "game/Board.h"
#include <string>
#include "Position.h"

class Board;

class Figure
{
public:
	Figure(Board* board,const unsigned int x,const unsigned int y,const bool figureColor);
	//Figure(Board* board, string pos);
	~Figure();
	
	//all theoretical moves
	virtual bool canMove(const char* to) const;
	virtual bool canMove(const Position p) const;
	virtual bool canEat(const Position p) const;

	virtual char* toString();
	virtual void setFigurePosition(const Position p);
	//checks if cell is empty or is occupied by figure with other color than this
	bool canReach(const Position p) const;

	//tries to move. if move is illegal, return false
	bool move(const Position p);
	virtual bool isMoved();

	const static bool WHITE = true;
	const static bool BLACK = false;
	bool figureColor;
	unsigned int x;
	unsigned int y;
	Board* board;
private:
	//checks if we can move with that figure without check
	bool isAllowedMove(const Position p);
};