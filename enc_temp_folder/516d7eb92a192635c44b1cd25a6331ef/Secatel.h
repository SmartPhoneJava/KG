#pragma once

typedef struct Secatel
{
	int Ymin;
	int Ymax;
	int Xmin;
	int Xmax;
} Secatel;

Secatel *newSecatel(int up, int right, int down, int left);

void deleteSecatel(Secatel **s);