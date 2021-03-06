#pragma once
#include "Figure.h"

class Pawn :
	public Figure
{
public:
	Pawn();
	~Pawn();
	Pawn(Board* board,const unsigned int x,const unsigned int y,const bool figureColor);
	bool canMove(const char* to) const;
	bool canMove(const Position p) const;
	bool canEat(const Position p) const;
	char* toString();
};

