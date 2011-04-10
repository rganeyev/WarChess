#pragma once

#include <string>
#include <vector>
#include "Figure.h"
#include "Player.h"
#include "Result.h"
#include "Position.h"

class Figure;


class Board {
public:
	Board(Player* white, Player* black, AMFWriter* streamWriter);
	~Board();
	//if move is legal, moves. otherwhise returns illegal move
	bool move(const char* from, const char* to);
	
	bool isCellEmpty(const Position p) const;
	Figure* getFigure(const Position p);
	void setFigure(Figure* figure, const Position p);

	//check, if any figure of that color can reach point x, y
	bool canFigureEatOnPosition(const Position p,const bool figureColor) const;
	
	Position whiteKingPos;
	Position blackKingPos;
	Position getKingPosition(const bool figureColor);
	bool getTurn() const;
private: 
	void addFiguresToBoard(bool figureColor, AMFWriter* streamWriter);
	void initFigures(AMFWriter* streamWriter);
	void initWhiteFigures(AMFWriter* streamWriter);
	void initBlackFigures(AMFWriter* streamWriter);

	bool checkForCheckMate();
	bool checkLegalMove();
	
	Player* whitePlayer;
	Player* blackPlayer;
	
	Figure* boardState[8][8];
	bool moveTurn;
};
