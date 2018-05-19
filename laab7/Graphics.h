#pragma once
#include <Windows.h>
#include "Secatel.h"
#include "Point.h"
#include "Alg.h"
#include "Cut.h"

#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 255, 0)
#define BLUE RGB(0, 0, 255)
#define BLACK RGB(0, 0, 0)
#define WHITE RGB(255, 255, 255)

void drawCut(HDC hdc, Cut cut, DWORD color);

void drawCutInside(HDC hdc, Cut cut,
	Secatel sec, DWORD color);

void drawSecatel(HDC hdc, Secatel sec,
	DWORD color);