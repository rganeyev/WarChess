#include "game/Figure.h"


Figure::Figure( Board* board, unsigned int x, unsigned int y, bool figureColor ): board(board), x(x), y(y), figureColor(figureColor)
{
	//do nothing
}

Figure::~Figure()
{
	//do nothing
}


bool Figure::canReach(const Position p ) const
{
	if (p.x > 7 || p.y > 7) {
		return false;
	}

	if (board->isCellEmpty(p)) {
		return true;
	}
	return (figureColor != board->getFigure(p)->figureColor);
}

bool Figure::canMove(const char* to ) const
{
	//assert(strlen(to) == 2);
	//заглушка
	return true;
}

bool Figure::canMove(const Position p ) const
{
	//заглушка
	return true;
}

bool Figure::canEat(const Position p ) const
{
	return true;
}

bool Figure::isAllowedMove(const Position p )
{
	if (!canMove(p)) {
		return false;
	}
		
	Position fromCell(this->x, this->y);
	//try to move
	Figure* pawnedFigure = board->getFigure(p);
	board->setFigure(this, p);
	board->setFigure(NULL, fromCell);

	bool ret = !board->canFigureEatOnPosition(board->getKingPosition(figureColor), !figureColor);

	board->setFigure(pawnedFigure, p);
	board->setFigure(this, fromCell);
	return ret;
}

bool Figure::move( Position p )
{
	if (!isAllowedMove(p)) {
		return false;
	}
	Figure* pawnedFigure = board->getFigure(p);
	board->setFigure(NULL, Position(this->x, this->y));

	setFigurePosition(p);
	
	board->setFigure(this, p);
	if (pawnedFigure != NULL) {
		delete pawnedFigure;
	}
	return true;
}

char* Figure::toString()
{
	return "Figure";
}

bool Figure::isMoved()
{
	return false;
}

void Figure::setFigurePosition( const Position p )
{
	x = p.x;
	y = p.y;
}
