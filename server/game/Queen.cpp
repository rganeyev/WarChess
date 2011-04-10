#include "Queen.h"


Queen::Queen( Board* board, const unsigned int x, const unsigned int y, const bool figureColor ): WaveFigure(board, x, y, figureColor)
{
}


bool Queen::canMove(const Position p ) const
{
	if (!canReach(p)) {
		return false;
	}
	return canWaveMove(p, 1, 0) || canWaveMove(p, 0, 1) || canWaveMove(p, -1, 0) || canWaveMove(p, 0, -1) ||
		canWaveMove(p, 1, -1) || canWaveMove(p, 1, 1) || canWaveMove(p, -1, -1) || canWaveMove(p, -1, 1);
}

bool Queen::canEat( const Position p ) const
{
	return canMove(p);
}


char* Queen::toString()
{
	return "Queen";
}

