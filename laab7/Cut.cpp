#include "stdafx.h"
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

Cut* newCut(Point a, Point b)
{
	Cut *cut = (Cut*)calloc(1, sizeof(Cut));
	cut->begin = a;
	cut->end = b;
	return cut;
}

void deleteCut(Cut **cut)
{
	free(*cut);
	*cut = NULL;
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

void debugCut(Cut cut, const char* text, int number)
{
	debug(text, number);
	debug("x1_cut", cut.begin.x);
	debug("y1_cut", cut.begin.y);
	debug("x2_cut", cut.end.x);
	debug("y2_cut", cut.end.y);
}