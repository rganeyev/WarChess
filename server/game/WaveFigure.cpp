#include "WaveFigure.h"

bool WaveFigure::canMove( const char* to, int dx, int dy )
{
	assert(strlen(to) == 2);
	unsigned int toX = to[1];
	unsigned int toY = to[0];
	unsigned int tmpX = x;
	unsigned int tmpY = y;
	while (canReach(tmpX, tmpY)) {
		if (tmpX == toX && tmpY == toY) {
			return true;
		}
		if (board->isCellEmpty(tmpX, tmpY)) {
			tmpX += dx;
			tmpX += dy;
		} else {
			break;
		}
	}
	return false;
}
