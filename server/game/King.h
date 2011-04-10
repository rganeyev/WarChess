#pragma once
#include "game/Figure.h"

class King: public Figure
{
public:
	King();
	King(Board* board, const unsigned int x, const unsigned int y, const bool figureColor);

	~King();


	bool canMove(const char* to) const;
	bool canMove(const Position p) const;
	bool canEat(const Position p) const;
	char* toString();
	void setFigurePosition(const Position p);
private: 
	bool moved;
};

