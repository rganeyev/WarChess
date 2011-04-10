#pragma once
#include "Figure.h"

class Knight: public Figure
{
public:
	Knight();
	Knight(Board* board, const unsigned int x, const unsigned int y, const bool figureColor);

	~Knight();


	bool canMove(const char* to) const;
	bool canMove(const Position p) const;
	bool canEat(const Position p) const;
	char* toString();
private:
	Position knightMoves[8];
};

