#include "stdafx.h"
#include "Graphics.h"

// Рисование отрезка по двум точкм, состоящим из 2 координат
void drawLine(HDC hdc, int X1, int Y1,
	int X2, int Y2, DWORD color)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, color);
	
	SelectObject(hdc, hPen);

	MoveToEx(hdc, X1, Y1, NULL);
	LineTo(hdc, X2, Y2);

	DeleteObject(hPen);
}

// Рисование двух структур точек
void draw2Points(HDC hdc, Point p1,
	Point p2, DWORD color)
{
	drawLine(hdc, p1.x, p1.y, p2.x, p2.y, color);
}

// Рисование структуры отрезка
void drawCut(HDC hdc, Cut cut, DWORD color)
{
	draw2Points(hdc, cut.begin, cut.end, color);
}

// Рисование отрезка внутри секатора
void drawCutInside(HDC hdc, Cut cut,
	Secatel sec, DWORD color)
{
	debugCut(*cutInside(cut, sec), "cutInside", 0);
	debugCut(*cutInside(cut, sec), "cutInside", 1);
	drawCut(hdc, *cutInside(cut, sec), color);
}

// Рисование секатора
void drawSecatel(HDC hdc, Secatel sec, DWORD color)
{
	Point *leftUp = newPoint(sec.Xmin, sec.Ymin);
	Point *leftDown = newPoint(sec.Xmin, sec.Ymax);
	Point *rightUp = newPoint(sec.Xmax, sec.Ymin);
	Point *rightDown = newPoint(sec.Xmax, sec.Ymax);

	draw2Points(hdc, *leftUp, *leftDown, color);
	draw2Points(hdc, *rightDown, *leftDown, color);
	draw2Points(hdc, *rightUp, *rightDown, color);
	draw2Points(hdc, *leftUp, *rightUp, color);
}