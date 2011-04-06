#include "game/Figure.h"


Figure::Figure( Board* board, unsigned int x, unsigned int y, bool figureColor ): board(board), x(x), y(y), figureColor(figureColor)
{
	//do nothing
}

Figure::~Figure()
{
	//do nothing
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

bool Figure::canMove( const char* to )
{
	//assert(strlen(to) == 2);
	//заглушка
	return true;
}

bool Figure::canMove( unsigned int x, unsigned int y )
{
	//заглушка
	return true;
}

bool Figure::isAllowedMove( unsigned int x, unsigned int y )
{
	if (!canMove(x, y)) {
		return false;
	}

	
	//try to move
	Figure* pawnedFigure = board->getFigure(x, y);
	board->setFigure(this, x, y);
	board->setFigure(NULL, this->x, this->y);

	bool ret = !board->canFigureReachPoint(board->getKingX(figureColor), board->getKingY(figureColor), !figureColor);

	board->setFigure(pawnedFigure, x, y);
	board->setFigure(this, this->x, this->y);
	return ret;
}

void Figure::move( unsigned int x, unsigned int y )
{
	Figure* pawnedFigure = board->getFigure(x, y);
	this->x = x;
	this->y = y;
	board->setFigure(this, x, y);
	if (pawnedFigure != NULL) {
		delete pawnedFigure;
	}
}
