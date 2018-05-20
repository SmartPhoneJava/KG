#pragma once
#include <windows.h>
#include <iostream>
#include <CommCtrl.h>
#include "Resource.h"
#include "Graphics.h"
#include "Table.h"

void setCharText(HWND hwnd, char* text);

void setIntText(HWND hWnd, int ID, int d);

void mouseClicked(LPARAM lParam, int borderX,
	int &count, int &x, int &y, bool can);

void deleteListViewItem(HWND listview, int i);

Table* adding(HWND hWnd, Table *table);

void updatingListView(HWND hWnd, LPARAM lParam,
	int &listview_choosen, int &listview_x1,
	int &listview_y1, int &listview_x2,
	int &listview_y2);

void deleting(HWND hWnd, int listview_choosen,
	int x1, int y1, int x2, int y2, Table* table);

void changing(HWND hWnd, int listview_choosen,
	Table* table, int old_x1, int old_y1,
	int old_x2, int old_y2);