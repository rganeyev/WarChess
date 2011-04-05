#include "game/Figure.h"


Figure::Figure( Board* board, unsigned int x, unsigned int y, bool figureColor ): board(board), x(x), y(y), figureColor(figureColor)
{
	//do nothing
}

Figure::~Figure()
{
	//do nothing
}


string Figure::toString()
{
	string ret;
	ret +=(char)('a' + y) + (char) (x+ '1');
	return ret;
}

bool Figure::canReach( unsigned int x, unsigned y )
{
	if (x > 7 || y > 7) {
		return false;
	}

	if (board->isCellEmpty(x, y)) {
		return true;
	}
	return (figureColor != board->getFigure(x, y)->figureColor);
}

bool Figure::canMove( string to )
{
	return true;
}

bool Figure::canMove( unsigned int x, unsigned int y )
{
	return true;
}

bool Figure::isAllowedMove( unsigned int x, unsigned int y )
{
	if (!canMove(x, y)) {
		return false;
	}

	bool ret = true;
	//try to move
	Figure* pawnedFigure = board->getFigure(x, y);
	board->setFigure(this, x, y);
	board->setFigure(NULL, this->x, this->y);

	for (unsigned int i = 0; i < 8; i++) {
		for (unsigned int j = 0; j < 8; j++) {
			if (!board->isCellEmpty(i ,j)) {
				Figure* tmp = board->getFigure(i, j);
				//if we open way to King, return false
				if (tmp->canMove(board->getKingX(!figureColor), board->getKingY(!figureColor))) {
					ret = false;
					goto isOver;
				}
			}
		}
	}

isOver:
	board->setFigure(pawnedFigure, x, y);
	board->setFigure(this, this->x, this->y);
	return ret;
}
