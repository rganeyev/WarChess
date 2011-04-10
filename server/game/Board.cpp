#include "Board.h"
#include "Pawn.h"
#include "King.h"
#include "Rook.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "WaveFigure.h"
#include "amf\AMFWriter.h"
#include "amf\AMFArrayWriter.h"
#include "amf\AMFReader.h"
#include "amf\AMFObjectWriter.h"


__declspec(thread) char intToStrBuffer[8];
String intToString(unsigned int i) {
	int len = sprintf(intToStrBuffer, "%d", i);
	return String(intToStrBuffer, len);
}

Board::Board(Player* white, Player* black, AMFWriter* streamWriter )
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

	initFigures(streamWriter);
	
	//TODO: implement fog
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

void addStringToArrayWriter(unsigned int index, char* name, char* value, AMFArrayWriter* writer) {
	AMFObjectWriter objWriter = writer->addObject(intToString(index));
	objWriter.begin();
		objWriter.writeUTF(String(name, strlen(name)), String(value, strlen(value)));
	objWriter.end();
}

void Board::initFigures(AMFWriter* streamWriter) {
	AMFObjectWriter figures = AMFObjectWriter(streamWriter);
	figures.begin();
	figures.writeArray(String("white", 5));
	initWhiteFigures(streamWriter);

	figures.writeArray(String("black", 5));
	initBlackFigures(streamWriter);

	figures.end();
}

char col(unsigned int i) {
	return (char)('a' + i);
}

char row(unsigned int i) {
	return (char)(i + '1');
}

void addFigureToWriter(unsigned int figureCount, char fig, unsigned int x, unsigned int y, AMFArrayWriter* writer) {
	char figureName[] = "Pa2";
	figureName[0] = fig;
	figureName[1] = col(y);
	figureName[2] = row(x);
	addStringToArrayWriter(figureCount, "fig", figureName, writer);
}

void Board::addFiguresToBoard(bool figureColor, AMFWriter* streamWriter) {
	char figureName[] ="Pa2";
	unsigned int startX = (figureColor == Figure::WHITE) ? 1 : 6;
	unsigned int figureCount = 0;
	AMFArrayWriter writer = AMFArrayWriter(streamWriter);
	writer.begin(16);
		for (unsigned int i = 0; i < 8; ++i) {	
			figureName[1] = col(i);
			figureName[2] = row(startX);
			addStringToArrayWriter(i, "fig", figureName, &writer);
			boardState[startX][i] = new Pawn(this, startX, i, figureColor);
			figureCount++;
		}

		
		startX = (figureColor == Figure::WHITE) ? 0 : 7;
		
		boardState[startX][4] = new King(this, startX, 4, figureColor);
		addFigureToWriter(figureCount++, 'K', startX, 4, &writer);

		boardState[startX][3] = new Queen(this, startX, 3, figureColor);
		addFigureToWriter(figureCount++, 'Q', startX, 3, &writer);

		
		boardState[startX][0] = new Rook(this, startX, 0, figureColor);
		addFigureToWriter(figureCount++, 'R', startX, 0, &writer);

		boardState[startX][7] = new Rook(this, startX, 7, figureColor);
		addFigureToWriter(figureCount++, 'R', startX, 7, &writer);

		boardState[startX][1] = new Knight(this, startX, 1, figureColor);
		addFigureToWriter(figureCount++, 'N', startX, 1, &writer);
		
		boardState[startX][6] = new Knight(this, startX, 6, figureColor);
		addFigureToWriter(figureCount++, 'N', startX, 6, &writer);

		boardState[startX][2] = new Bishop(this, startX, 2, figureColor);
		addFigureToWriter(figureCount++, 'B', startX, 2, &writer);
		
		boardState[startX][5] = new Bishop(this, startX, 5, figureColor);
		addFigureToWriter(figureCount++, 'B', startX, 5, &writer);
	writer.end();
}

void Board::initWhiteFigures(AMFWriter* streamWriter)
{
	addFiguresToBoard(Figure::WHITE, streamWriter);
}


void Board::initBlackFigures(AMFWriter* streamWriter)
{
	addFiguresToBoard(Figure::BLACK, streamWriter);
}


bool Board::isCellEmpty(const Position p ) const
{
	assert(p.x <= 7 && p.y <= 7);
	return (boardState[p.x][p.y] == NULL);
}

Figure* Board::getFigure(const Position p )
{
	assert( p.x <= 7 && p.y <= 7);
	return boardState[p.x][p.y];
}

bool Board::canFigureEatOnPosition(const Position p,const bool figureColor) const
{
	for (unsigned int i = 0; i < 8; i++) {
		for (unsigned int j = 0; j < 8; j++) {
			if (boardState[i][j] != NULL && boardState[i][j]->figureColor == figureColor) {
				if (boardState[i][j]->canEat(p)) {
					return true;
				}
			}
		}
	}
	return false;
}

void Board::setFigure(Figure* figure,const Position p )
{
	boardState[p.x][p.y] = figure;
	if (figure != NULL) {
		figure->setFigurePosition(p);
	}
}

bool Board::move(const char* from,const char* to )
{

	unsigned int fromX = from[1] - '1';
	unsigned int fromY = from[0] - 'a';
	unsigned int toX = to[1] - '1';
	unsigned int toY = to[0] - 'a';
	
	Figure* figure = getFigure(Position(fromX, fromY));
	if (figure == NULL) {
		return false;
	}

	if (figure->figureColor != moveTurn) {
		return false;
	}

	if (figure->move(Position(toX, toY))) {

		//change turn
		moveTurn = !moveTurn;

		return true;
	} else {
		return false;
	}
}

bool Board::getTurn() const
{
	return moveTurn;
}

Position Board::getKingPosition(const bool figureColor )
{
	return figureColor == Figure::WHITE ? whiteKingPos : blackKingPos;
}
