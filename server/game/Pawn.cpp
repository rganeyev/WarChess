#include "Pawn.h"


Pawn::Pawn( Board* board, unsigned int x, unsigned int y, bool figureColor ): Figure(board, x, y, figureColor)
{
	//do nothing
}


Pawn::~Pawn()
{
	//do nothing
}


bool Pawn::canMove( const char* to )
{
	//assert(strlen(to) == 2);

	unsigned int toX = to[1];
	unsigned int toY = to[0];
	
	return canMove(toX, toY);
}

bool Pawn::canMove( unsigned int x, unsigned int y )
{
	if (abs((int)(y - this->y)) > 1) {
		return false;
	}

	if (this->y == y) {
		if (abs((int)(x - this->x)) > 2) {
			return false;
		}
		//try to move forward
		if (!board->isCellEmpty(x, y)) {
			return false;
		}

		if (figureColor == Figure::WHITE) {
			return (this->x == 1 && x == 3) ? board->isCellEmpty(2, y) : true;
		} else {
			return (this->x == 6 && x == 4) ? board->isCellEmpty(5, y) : true;
		}
	} else {
		int direction = (figureColor == Figure::WHITE) ?  1 : -1;
		return (x - this->x == direction) && (!board->isCellEmpty(x, y));
	}
}
