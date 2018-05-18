#pragma once
#include "Visibility.h"

#define MODE_LEFT 1
#define MODE_RIGHT 2
#define MODE_UP 3
#define MODE_DOWN 4

typedef struct Point
{
	int x;
	int y;
	Visibility vis;
} Point;

Point* newPoint(int x, int y);

void deletePoint(Point **point);

void setPointVisibility(Point p, Secatel s);