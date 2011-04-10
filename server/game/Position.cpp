#include "Position.h"

Position::Position( unsigned int x, unsigned int y ): x(x), y(y)
{
	//do nothing
}

Position::Position()
{
	x = 0;
	y = 0;
}

bool Position::operator==( Position other ) const
{
	return (x == other.x) && (y == other.y);
}
