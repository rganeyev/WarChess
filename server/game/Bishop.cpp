#include "Bishop.h"


Bishop::Bishop( Board* board, const unsigned int x, const unsigned int y, const bool figureColor ): WaveFigure(board, x, y, figureColor)
{
	//do nothing
}


bool Bishop::canMove(const Position p ) const
{
	if (!canReach(p)) {
		return false;
	}
	return canWaveMove(p, 1, -1) || canWaveMove(p, 1, 1) || canWaveMove(p, -1, -1) || canWaveMove(p, -1, 1);
}

bool Bishop::canEat( const Position p ) const
{
	return canMove(p);
}

char* Bishop::toString()
{
	return "Bishop";
}



