#include "stdafx.h"
#include "Secatel.h"

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

void debugSecatel(Secatel secatel,
	const char* text, int number)
{
	debug(text, number);
	debug("secatel.Xmax", secatel.Xmax);
	debug("secatel.Xmin", secatel.Xmin);
	debug("secatel.Ymin", secatel.Ymin);
	debug("secatel.Ymax", secatel.Ymax);
}