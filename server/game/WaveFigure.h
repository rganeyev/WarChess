#include "game/Figure.h"

class WaveFigure: public Figure
{
public:
	WaveFigure();
	~WaveFigure();
	bool canMove(string to, int dx, int dy);
};