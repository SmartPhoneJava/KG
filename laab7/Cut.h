#pragma once

#include "Point.h"
#include "Visibility.h"
#include "Debug.h"

#define VERTICAL 1000000
#define HORIZONTAL 0

typedef struct Cut
{
	Point begin;
	Point end;
} Cut;

double cutTan(Cut cut);

Cut* newCut(Point a, Point b);

void deleteCut(Cut **cut);

void debugCut(Cut cut, const char* text, int number);

void debugCutVisibility(Cut cut, const char* text, int number);

