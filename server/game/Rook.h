#pragma once
#include "WaveFigure.h"

class Rook :
	public WaveFigure
{
public:
	Rook(Board* board,const unsigned int x,const unsigned int y,const bool figureColor);
	~Rook();
	bool canMove(const Position p) const;
	bool canEat(const Position p) const;
	void setFigurePosition(const Position p);
	bool isMoved();
	char* toString();
private:
	bool moved;
};

