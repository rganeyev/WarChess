#pragma once
#include "WaveFigure.h"

class Bishop :
	public WaveFigure
{
public:
	Bishop(Board* board,const unsigned int x,const unsigned int y,const bool figureColor);
	~Bishop();
	bool canMove(const Position p) const;
	bool canEat(const Position p) const;
	char* toString();
};

