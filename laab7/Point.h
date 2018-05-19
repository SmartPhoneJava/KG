#pragma once
#include "Visibility.h"
#include "Secatel.h"

#define NO_POINT -1

typedef struct Point
{
	int x;
	int y;
	Visibility vis;
} Point;

Point* newPoint(int x, int y);

void deletePoint(Point **point);

void setPointVisibility(Point p, Secatel s);

bool isPointFree(Point point);