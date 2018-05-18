#include "Point.h"
#include "Cut.h"
#include "Alg.h"
#include "Secatel.h"
#include <iostream>

int count(double tangens, int coord_secator,
	int coord1_point, int coord2_point)
{
	return tangens * (coord_secator - coord1_point) + coord2_point;
}

Point* whoLeft(Cut cut)
{
	if (cut.begin.vis.T1 == 1)
		return &(cut.begin);
	if (cut.end.vis.T1 == 1)
		return &(cut.end);
	return NULL;
}

Point* whoRight(Cut cut)
{
	if (cut.begin.vis.T2 == 1)
		return &(cut.begin);
	if (cut.end.vis.T2 == 1)
		return &(cut.end);
	return NULL;
}

Point* whoUp(Cut cut)
{
	if (cut.begin.vis.T3 == 1)
		return &(cut.begin);
	if (cut.end.vis.T3 == 1)
		return &(cut.end);
	return NULL;
}

Point* whoDown(Cut cut)
{
	if (cut.begin.vis.T4 == 1)
		return &(cut.begin);
	if (cut.end.vis.T4 == 1)
		return &(cut.end);
	return NULL;
}

// a <= b <= c
bool checkBorder(int a, int b, int c)
{
	return ((a <= b) && (b <= c));
}

Point* borderLeft(Point *p, Secatel sec, double m)
{
	int x_new = sec.Xmin;
	int y_new = NO_POINT;

	if ((m == HORIZONTAL) || (m == VERTICAL))
		y_new = p->y;
	else
		y_new = count(m, x_new, p->x, p->y);

	if (checkBorder(int a, int b, int c))
	return newPoint(x_new, y_new);
}

Point* borderRight(Point *p, Secatel sec, double m)
{
	int x_new = sec.Xmax;
	int y_new = NO_POINT;

	if ((m == HORIZONTAL) || (m == VERTICAL))
		y_new = p->y;
	else
		y_new = count(m, x_new, p->x, p->y);
	return newPoint(x_new, y_new);
}

Point* borderUp(Point *p, Secatel sec, double m)
{
	int x_new = NO_POINT;
	int y_new = sec.Ymin;

	if ((m == HORIZONTAL) || (m == VERTICAL))
		x_new = p->x;
	else
		x_new = count(1 / m, y_new, p->y, p->x);
	return newPoint(x_new, y_new);
}

Point* borderDown(Point *p, Secatel sec, double m)
{
	int x_new = NO_POINT;
	int y_new = sec.Ymax;

	if ((m == HORIZONTAL) || (m == VERTICAL))
		x_new = p->x;
	else 
		x_new = count(1 / m, y_new, p->y, p->x);
	return newPoint(x_new, y_new);
}

typedef Point*(*Border)(Point *p, Secatel sec, double m);
typedef Point*(*Who)(Cut cut);

Point* move(Border border, Who who, Cut cut,
	Secatel sec, double tg, int mode)
{
	Point *point = who(cut);
	Point *result = NULL;
	if (point)
		result = border(point, sec, tg);
	
}

void cutInside(Cut cut, Secatel sec,
	Point* first, Point *second)
{
	double tg = cutTan(cut);
	Point *point; 
	point = whoLeft(cut);
	if (point)
		borderLeft(point, sec, tg);
}