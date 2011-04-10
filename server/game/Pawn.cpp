#include "Pawn.h"


Pawn::Pawn( Board* board, const unsigned int x, const unsigned int y, const bool figureColor ): Figure(board, x, y, figureColor)
{
	//do nothing
}


Pawn::~Pawn()
{
	//do nothing
}


bool Pawn::canMove(const char* to ) const
{
	//assert(strlen(to) == 2);

	unsigned int toX = to[1];
	unsigned int toY = to[0];
	
	return canMove(Position(toX, toY));
}

bool Pawn::canMove(const Position p ) const
{
	if (!canReach(p)) {
		return false;
	}
	if (abs((int)(p.y - this->y)) > 1) {
		return false;
	}

	if (this->y == p.y) {
		if (abs((int)(p.x - this->x)) > 2) {
			return false;
		}
		//try to move forward
		if (!board->isCellEmpty(p)) {
			return false;
		}

		if (figureColor == Figure::WHITE) {
			if (p.x - this->x == 2) {
				return (this->x == 1 && p.x == 3) && board->isCellEmpty(Position(2, p.y));
			} else {
				return p.x - this->x == 1;
			}
		} else {
			if (this->x - p.x == 2) {
				return (this->x == 6 && p.x == 4) && board->isCellEmpty(Position(5, p.y));
			} else {
				return this->x - p.x == 1;
			}
		}
	} else {
		int direction = (figureColor == Figure::WHITE) ?  1 : -1;
		return (p.x - this->x == direction) && (!board->isCellEmpty(p));
	}
}

bool Pawn::canEat(const Position p ) const
{
	int direction = (figureColor == Figure::WHITE) ?  1 : -1;
	return abs((int)(x - p.x) == 1) && (p.x - this->x == direction) && (!board->isCellEmpty(p));
}

char* Pawn::toString()
{
	return "Pawn";
}

