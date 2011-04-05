#include "Board.h"
#include "Pawn.h"


Board::Board( Player* white, Player* black )
{
	whitePlayer = white;
	blackPlayer = black;

	white->board = this;
	black->board = this;
	
	//init white and black figures
	initWhiteFigures();

	initBlackFigures();

	//TODO: implement fog

	whiteKingX = 4;
	blackKingX = 4;
	whiteKingY = 0;
	blackKingY = 7;
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
	//TODO: implement other figures
}


void Board::initBlackFigures()
{
	for (unsigned int i = 0; i < 8; i++) {
		boardState[1][i] = new Pawn(this, 1, i, Figure::BLACK);
	}
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

bool Board::canFigureMove( unsigned int x, unsigned int y, bool figureColor)
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
