#pragma once

#include <string>
#include <vector>
#include "game/Figure.h"
#include "game/Player.h"
#include "game\Result.h"

class Figure;

class Board {
public:
	Board(Player* white, Player* black);
	~Board();
	//if move is legal, moves. otherwhise returns illegal move
	Result move(const char* from, const char* to);
	
	bool isCellEmpty(unsigned int x, unsigned int y);
	Figure* getFigure(unsigned int x, unsigned int y);
	void setFigure(Figure* figure, unsigned int x, unsigned int y);

	//check, if any figure of that color can reach point x, y
	bool canFigureReachPoint(unsigned int x, unsigned int y, bool figureColor);
	
	unsigned int getKingX(bool figureColor);
	unsigned int getKingY(bool figureColor);
	bool isLeftRookMoved(bool figureColor);
	bool isRightRookMoved(bool figureColor);
	bool getTurn();
private: 
	void initWhiteFigures();
	void initBlackFigures();
	bool checkForCheckMate();
	bool checkLegalMove();
	
	Player* whitePlayer;
	Player* blackPlayer;
	
	Figure* boardState[8][8];

	unsigned int whiteKingX;
	unsigned int whiteKingY;
	unsigned int blackKingX;
	unsigned int blackKingY;
	bool whiteLeftRookMoved;
	bool whiteRightRookMoved;
	bool blackLeftRookMoved;
	bool blackRightRookMoved;
	bool moveTurn;
};
