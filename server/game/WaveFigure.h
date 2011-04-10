#pragma once

#include "game/Figure.h"

class WaveFigure: public Figure
{
public:
	WaveFigure(Board* board,const unsigned int x,const unsigned int y,const bool figureColor);
	~WaveFigure();
	bool canWaveMove(const Position p, const int dx,const int dy) const;
	bool canEat(const Position p) const;

	virtual bool canMove(const Position p) const;
	virtual char* toString();
	virtual bool isMoved();
	virtual void setFigurePosition(const Position p);
};