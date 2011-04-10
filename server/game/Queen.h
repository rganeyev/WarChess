#pragma once
#include "WaveFigure.h"

class Queen :
	public WaveFigure
{
public:
	Queen(Board* board,const unsigned int x,const unsigned int y,const bool figureColor);
	~Queen();
	bool canMove(const Position p) const;
	bool canEat(const Position p) const;
	char* toString();
};

