#pragma once
struct Position 
{
	Position();
	Position(unsigned int x, unsigned int y);
	unsigned int x;
	unsigned int y;
	bool operator ==(Position other) const;
};
