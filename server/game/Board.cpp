#include "Board.h"
#include "Pawn.h"
#include "King.h"


Board::Board( Player* white, Player* black )
{
	whitePlayer = white;
	blackPlayer = black;

	white->board = this;
	black->board = this;
	
	//init white and black figures
	for (unsigned int i = 0; i < 8; i++) {
		for( unsigned int j = 0; j < 8; j++) {
			boardState[i][j] = NULL;
		}
	}

	initWhiteFigures();

	initBlackFigures();

	//TODO: implement fog

	whiteKingX = 0;
	blackKingX = 7;
	whiteKingY = 4;
	blackKingY = 4;

	whiteLeftRookMoved = false;
	whiteRightRookMoved = false;
	blackRightRookMoved = false;
	blackLeftRookMoved = false;

	moveTurn = Figure::WHITE;
}

Board::~Board()
{

	for (unsigned int i = 0; i < 8; i++) {
		for (unsigned int j = 0; j < 8; j++) {
			if (boardState[i][j] != NULL){
				delete boardState[i][j];	
			}
		}
	}
}

void Board::initWhiteFigures()
{
	for (unsigned int i = 0; i < 8; i++) {
		boardState[1][i] = new Pawn(this, 1, i, Figure::WHITE);
	}
	boardState[0][4] = new King(this, 0, 4, Figure::WHITE);
	//TODO: implement other figures
}


void Board::initBlackFigures()
{
	for (unsigned int i = 0; i < 8; i++) {
		boardState[6][i] = new Pawn(this, 6, i, Figure::BLACK);
	}
	boardState[7][4] = new King(this, 7, 4, Figure::BLACK);
	//TODO: implement other figures
}


bool Board::isCellEmpty( unsigned int x, unsigned int y )
{
	assert(x >= 0 && x <= 7 && y >= 0 && y <= 7);
	return (boardState[x][y] == NULL);
}

Figure* Board::getFigure( unsigned int x, unsigned int y )
{
	assert(x >= 0 && x <= 7 && y >= 0 && y <= 7);
	return boardState[x][y];
}

bool Board::canFigureReachPoint( unsigned int x, unsigned int y, bool figureColor)
{
	for (unsigned int i = 0; i < 8; i++) {
		for (unsigned int j = 0; j < 8; j++) {
			if (boardState[i][j] != NULL && boardState[i][j]->figureColor == figureColor) {
				if (boardState[i][j]->canMove(x, y)) {
					return true;
				}
			}
		}
	}
	return false;
}

unsigned int Board::getKingX( bool figureColor )
{
	return figureColor == Figure::WHITE ? whiteKingX : blackKingX;
}

unsigned int Board::getKingY( bool figureColor )
{
	return figureColor == Figure::WHITE ? whiteKingY : blackKingY;
}

void Board::setFigure( Figure* figure, unsigned int x, unsigned int y )
{
	boardState[x][y] = figure;
}

Result Board::move(const char* from,const char* to )
{
	//assert(strlen(from) == 2);
	//assert(strlen(to) == 2);

	unsigned int fromX = from[1] - '1';
	unsigned int fromY = from[0] - 'a';
	unsigned int toX = to[1] - '1';
	unsigned int toY = to[0] - 'a';
	if (isCellEmpty(fromX, fromY)) {
		return IllegalMove;
	}

	Figure* figure = getFigure(fromX, fromY);

	if (figure->figureColor != moveTurn) {
		return IllegalMove;
	}

	if (figure->isAllowedMove(toX, toY)) {
		figure->move(toX, toY);
		//change turn

		moveTurn = !moveTurn;

		return Success;
	} else {
		return IllegalMove;
	}
}

bool Board::isLeftRookMoved( bool figureColor )
{
	return figureColor == Figure::WHITE ? whiteLeftRookMoved : blackLeftRookMoved;
}

bool Board::isRightRookMoved( bool figureColor )
{
	return figureColor == Figure::WHITE ? whiteRightRookMoved : blackRightRookMoved;
}

bool Board::getTurn()
{
	return moveTurn;
}
