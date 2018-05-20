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

void drawEllipse(HDC hdc, int X,
	int Y, int rad, DWORD color)
{
	HBRUSH hBrush = CreateSolidBrush(color);

	SelectObject(hdc, hBrush);
	Ellipse(hdc, X - rad, Y - rad, X + rad, Y + rad);
	DeleteObject(hBrush);
}

// Рисование точки
void drawPoint(HDC hdc, Point *p,
	int rad, DWORD color)
{
	drawEllipse(hdc, p->x,
		p->y, rad, color);
}

// Рисование двух структур точек
void draw2Points(HDC hdc, Point *p1,
	Point *p2, DWORD color)
{
	if (isPointFree(p1))
		drawPoint(hdc, p1, 3, color);
	else if (isPointFree(p2))
		drawPoint(hdc, p2, 3, color);
	else
	{
		drawLine(hdc, p1->x, p1->y, p2->x, p2->y, color);
	}
}

// Рисование структуры отрезка
void drawCut(HDC hdc, Cut *cut, DWORD color)
{
	draw2Points(hdc, cut->begin, cut->end, color);
}

// Рисование отрезка внутри секатора
void drawCutInside(HDC hdc, Cut *cut,
	Secatel sec, DWORD color)
{
	debugCut(cutInside(cut, sec), "cutInside", 0);
	debugCut(cutInside(cut, sec), "cutInside", 1);
	drawCut(hdc, cutInside(cut, sec), color);
}

// Рисование секатора
void drawSecatel(HDC hdc, Secatel sec, DWORD color)
{
	Point *leftUp = newPoint(sec.Xmin, sec.Ymin);
	Point *leftDown = newPoint(sec.Xmin, sec.Ymax);
	Point *rightUp = newPoint(sec.Xmax, sec.Ymin);
	Point *rightDown = newPoint(sec.Xmax, sec.Ymax);

	draw2Points(hdc, leftUp, leftDown, color);
	draw2Points(hdc, rightDown, leftDown, color);
	draw2Points(hdc, rightUp, rightDown, color);
	draw2Points(hdc, leftUp, rightUp, color);
}

// Отрисовка отрезков
void drawTable(HDC hdc, Table* table, DWORD color)
{
	Table* mov = table;
	while (mov != NULL)
	{
		drawCut(hdc, mov->cut, color);
		mov = mov->next;
	}
}

void cleanRectOld(HWND hWnd, LONG left,
	LONG top, LONG right, LONG bottom)
{
	PAINTSTRUCT ps;
	RECT rect;
	HDC hdc = GetDC(hWnd);

	GetClientRect(hWnd, &rect);

	if (left > 0)
		rect.left = left;
	if (right > 0)
		rect.right = rect.right - right;
	if (top > 0)
		rect.top = top;
	if (bottom > 0)
		rect.bottom = bottom;

	FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
	ReleaseDC(hWnd, hdc);
}

RECT getRect(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	return rect;
}

void cleanRect(HDC hdc, RECT rect)
{
	FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
}

void cleanDisplay(HWND hWnd)
{
	RECT rect = getRect(hWnd);
	HDC hdc = GetDC(hWnd);
	cleanRect(hdc, rect);
	ReleaseDC(hWnd, hdc);
}