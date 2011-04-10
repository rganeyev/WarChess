#include "King.h"


King::King( Board* board, const unsigned int x, const unsigned int y, const bool figureColor ): Figure(board, x, y, figureColor)
{
	if (figureColor == Figure::WHITE) {
		board->whiteKingPos = Position(x, y);
	} else {
		board->blackKingPos = Position(x, y);
	}
	moved = false;
}


King::~King()
{
}

bool King::canMove(const Position p) const
{
	if (!canReach(p)) {
		return false;
	}
	unsigned int startX = figureColor == Figure::WHITE ? 0 : 7;
	
	
	//короткая рокировка
	if (!moved && p.x == startX && p.y == 6) {
		Figure* rook = board->getFigure(Position(startX, 7));
		if (rook == NULL || strcmp("Rook", rook->toString()) == 0 || rook->isMoved() || rook->figureColor != figureColor) {
			return false;
		}
		for (unsigned int j = 4; j < 7; j++) {
			if (!board->isCellEmpty(Position(startX, j))  || board->canFigureEatOnPosition(Position(startX, j), !figureColor)) {
				return false;
			}
		}
		return true;
	} 

	//длинная рокировка
	if (!moved && p.x == startX && p.y == 2) {
		Figure* rook = board->getFigure(Position(startX, 0));
		if (rook == NULL || strcmp("Rook", rook->toString()) == 0 || rook->isMoved() || rook->figureColor != figureColor) {
			return false;
		}
		for (unsigned int j = 2; j < 5; j++) {
			if (!board->isCellEmpty(Position(startX, j))  || board->canFigureEatOnPosition(Position(startX, j), !figureColor)) {
				return false;
			}
		}
		return true;
	}
	
	return (abs((int)(p.x - this->x)) < 2) && (abs((int)(p.y - this->y)) < 2);
}

bool King::canMove( const char* to ) const
{
	//assert (strlen(to) == 2);

	unsigned int toX = to[1] - '1';
	unsigned int toY = to[0] - 'a';

	return canMove(Position(toX, toY));

}

bool King::canEat( const Position p ) const
{
	return (abs((int)(p.x - this->x)) < 2) && (abs((int)(p.y - this->y)) < 2);
}

char* King::toString()
{
	return "King";
}

void King::setFigurePosition( const Position p )
{
	Figure::setFigurePosition(p);
	moved = true;
	if (figureColor == Figure::WHITE) {
		board->whiteKingPos = Position(x, y);
	} else {
		board->blackKingPos = Position(x, y);
	}	
}
