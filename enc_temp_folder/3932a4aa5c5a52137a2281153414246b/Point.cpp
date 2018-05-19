#include "stdafx.h"
#include "Point.h"
#include "Secatel.h"
#include <iostream>

Point* newPoint(int x, int y)
{
	Point* point = (Point*)calloc(1, sizeof(Point));
	point->x = x;
	point->y = y;
	return point;
}

void deletePoint(Point **point)
{
	free(*point);
	*point = NULL;
}

bool isPointFree(Point point)
{
	bool x = (point.x == NO_POINT);
	bool y = (point.y == NO_POINT);
	return (x && y);
}

void setPointVisibility(Point p, Secatel s)
{
	if (p.x < s.Xmin)  
		p.vis.T1 = 1;
	else
		p.vis.T1 = 0;

	if (p.x > s.Xmax)  
		p.vis.T2 = 1;
	else
		p.vis.T2 = 0;

	if (p.y < s.Ymin)
		p.vis.T3 = 1;
	else
		p.vis.T3 = 0;

	if (p.y > s.Ymax)  
		p.vis.T4 = 1;
	else
		p.vis.T4 = 0;
}