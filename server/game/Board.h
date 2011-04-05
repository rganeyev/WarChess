#pragma once

#include <string>
#include <vector>
#include "game/Figure.h"
#include "game/Player.h"

class Figure;
using std::string;

class Board {
public:
	Board(Player* white, Player* black);
	~Board();
	void getState();
	void move(string from, string to);
	bool isCellEmpty(unsigned int x, unsigned int y);
	Figure* getFigure(unsigned int x, unsigned int y);
	void setFigure(Figure* figure, unsigned int x, unsigned int y);
	bool canFigureMove(unsigned int x, unsigned int y, bool figureColor);
	unsigned int getKingX(bool figureColor);
	unsigned int getKingY(bool figureColor);
private: 
	void initWhiteFigures();
	void initBlackFigures();
	bool checkForCheck();
	bool checkForCheckMate();
	bool checkLegalMove();
	
	Player* whitePlayer;
	Player* blackPlayer;
	
	Figure* boardState[8][8];

	unsigned int whiteKingX;
	unsigned int whiteKingY;
	unsigned int blackKingX;
	unsigned int blackKingY;
};
