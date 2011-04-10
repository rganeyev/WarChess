#include "Rook.h"


Rook::Rook( Board* board, const unsigned int x, const unsigned int y, const bool figureColor ): WaveFigure(board, x, y, figureColor)
{
	moved = false;
}


bool Rook::canMove(const Position p ) const
{
	if (!canReach(p)) {
		return false;
	}
	return canWaveMove(p, 1, 0) || canWaveMove(p, 0, 1) || canWaveMove(p, -1, 0) || canWaveMove(p, 0, -1);
}

bool Rook::canEat( const Position p ) const
{
	return canMove(p);
}

char* Rook::toString()
{
	return "Rook";
}

bool Rook::isMoved()
{
	return moved;
}

void Rook::setFigurePosition( const Position p )
{
	Figure::setFigurePosition(p);
	moved = true;
}

