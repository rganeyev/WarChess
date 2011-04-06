#pragma once
#include "game\Figure.h"

class Pawn :
	public Figure
{
public:
	Pawn();
	~Pawn();
	Pawn(Board* board, unsigned int x, unsigned int y, bool figureColor);
	bool canMove(const char* to);
	bool canMove(unsigned int x, unsigned int y);
};

