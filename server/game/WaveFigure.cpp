#include "WaveFigure.h"

WaveFigure::WaveFigure( Board* board,const unsigned int x,const unsigned int y,const bool figureColor ) : Figure(board, x, y, figureColor)
{
	//do nothing
}

bool WaveFigure::canWaveMove(const Position p,const int dx, const int dy ) const
{
	Position pos(x + dx, y + dy);
	while (canReach(pos)) {
		if (pos == p) {
			return true;
		}
		if (board->isCellEmpty(pos)) {
			pos.x += dx;
			pos.y += dy;
		} else {
			break;
		}
	}
	return false;
}

bool WaveFigure::canEat(const Position p) const
{
	return canMove(p);
}

bool WaveFigure::canMove(const Position p ) const
{
	return false;
}

char* WaveFigure::toString()
{
	return "WaveFigure";
}

bool WaveFigure::isMoved()
{
	return false;
}

void WaveFigure::setFigurePosition( const Position p )
{
	Figure::setFigurePosition(p);
}
