#include "Knight.h"


Knight::Knight( Board* board, const unsigned int x, const unsigned int y, const bool figureColor ): Figure(board, x, y, figureColor)
{
	
	knightMoves[0] = Position(-2, -1);
	knightMoves[1] = Position(-2, 1);
	knightMoves[2] = Position(-1, -2);
	knightMoves[3] = Position(-1, 2);
	knightMoves[4] = Position(1, -2); 
	knightMoves[5] = Position(1, 2);
	knightMoves[6] = Position(2, -1);
	knightMoves[7] = Position(2, 1);
	//do nothing
}


Knight::~Knight()
{
	
}

bool Knight::canMove(const Position p) const
{
	if (!canReach(p)) {
		return false;
	}
	for (unsigned int i = 0; i < 8; i++) {
		if (p == Position(knightMoves[i].x + x, knightMoves[i].y + y)) {
			return true;
		}
	}
	return false;
}

bool Knight::canMove( const char* to ) const
{
	unsigned int toX = to[1] - '1';
	unsigned int toY = to[0] - 'a';

	return canMove(Position(toX, toY));

}

bool Knight::canEat( const Position p ) const
{
	return canMove(p);
}

char* Knight::toString()
{
	return "Knight";
}
