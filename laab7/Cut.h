#pragma once

#include "Point.h"
#include "Visibility.h"

#define VERTICAL 1000000
#define HORIZONTAL 0

typedef struct Cut
{
	Point begin;
	Point end;
} Cut;

double cutTan(Cut cut);