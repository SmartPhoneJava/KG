#include "Cut.h"
#include <iostream>

int getCutDX(Cut cut)
{
	return cut.end.x - cut.begin.x;
}

int getCutDY(Cut cut)
{
	return cut.end.y - cut.begin.y;
}

double cutTan(Cut cut)
{
	int dx = getCutDX(cut);
	int dy = getCutDY(cut);

	if (dx == 0)
		return VERTICAL;
	else if (dy == 0)
		return HORIZONTAL;
	else
	{
		return (double)dy/(double)dx;
	}
}