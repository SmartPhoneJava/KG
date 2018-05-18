#include "Secatel.h"
#include <iostream>

Secatel* newSecatel(int up, int right, int down, int left)
{
	Secatel *secatel = (Secatel*)calloc(1, sizeof(Secatel));
	secatel->Xmax = right;
	secatel->Xmin = left;
	secatel->Ymax = down;
	secatel->Ymin = up;
	
	return secatel;
}

void deleteSecatel(Secatel **s)
{
	free(*s);
	*s = NULL;
}