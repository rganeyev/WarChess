#include "game/Figure.h"

class WaveFigure: public Figure
{
public:
	WaveFigure();
	~WaveFigure();
	bool canMove(const char* to, int dx, int dy);
};