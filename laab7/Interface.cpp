#include "stdafx.h"
#include "Interface.h"

// Установка обычного текста
// https://stackoverflow.com/questions/6858524/convert-char-to-lpwstr
// https://stackoverflow.com/questions/29847036/convert-char-to-wchar-t-using-mbstowcs-s
void setCharText(HWND hwnd, char* text)
{
	wchar_t wtext[BUFFER_MAX_SIZE];
	size_t outSize;
	size_t size = strlen(text) + 1;
	mbstowcs_s(&outSize, wtext, size, text, size - 1);//Plus null
	LPWSTR ptr = wtext;
	SetWindowText(hwnd, text);
}

// Установка текстом целочисленных значений
void setIntText(HWND hWnd, int ID, int d)
{
	TCHAR buffer[BUFFER_MAX_SIZE];
	_snprintf_s(buffer, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", d);
	setCharText(GetDlgItem(hWnd, ID), buffer);
}

/*

borderX - максмальное значения X
x, y - значения параметров которые обновятся
count - счётчик кликов
*/
void mouseClicked(LPARAM lParam, int borderX,
	int &count, int &x, int &y, bool can)
{
	if ((LOWORD(lParam) < borderX) && can)
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		count++;
	}
}

void addListViewItem(HWND ListView, int x1, int y1, int x2, int y2)
{
	LVITEM lis;
	lis.lParam = 0;

	TCHAR buffer_x1[BUFFER_MAX_SIZE];
	TCHAR buffer_y1[BUFFER_MAX_SIZE];
	TCHAR buffer_x2[BUFFER_MAX_SIZE];
	TCHAR buffer_y2[BUFFER_MAX_SIZE];

	_snprintf_s(buffer_x1, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", x1);

	_snprintf_s(buffer_y1, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", y1);

	_snprintf_s(buffer_x2, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", x2);

	_snprintf_s(buffer_y2, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", y2);

	memset(&lis, 0, sizeof(LV_ITEM));

	int row = 0;
	lis.iItem = row;
	lis.iSubItem = 0;
	ListView_InsertItem(ListView, &lis);
	ListView_SetItemText(ListView, row, 0, buffer_x1);

	lis.iSubItem = 1;
	ListView_InsertItem(ListView, &lis);
	ListView_SetItemText(ListView, row, 1, buffer_y1);

	lis.iSubItem = 2;
	ListView_InsertItem(ListView, &lis);
	ListView_SetItemText(ListView, row, 2, buffer_x2);

	lis.iSubItem = 3;
	ListView_InsertItem(ListView, &lis);
	ListView_SetItemText(ListView, row, 3, buffer_y2);
}

void deleteListViewItem(HWND listview, int i)
{
	ListView_DeleteItem(listview, i);
}

// Получение текста как чисел
int getIntText(HWND hWnd, int ID)
{
	TCHAR buffer[BUFFER_MAX_SIZE];
	GetWindowText(GetDlgItem(hWnd, ID), buffer, sizeof(buffer));
	return atoi(buffer);
}

// Получение координат
void getCoordinates(HWND hWnd, int &x1, int &y1, int &x2, int &y2)
{
	x1 = getIntText(hWnd, ID_EDIT_X1);
	y1 = getIntText(hWnd, ID_EDIT_Y1);
	x2 = getIntText(hWnd, ID_EDIT_X2);
	y2 = getIntText(hWnd, ID_EDIT_Y2);
}

Table* adding(HWND hWnd, Table *table)
{
	int x1 = NO_POINT, x2 = NO_POINT;
	int y1 = NO_POINT, y2 = NO_POINT;

	getCoordinates(hWnd, x1, y1, x2, y2);

	HWND listview = GetDlgItem(hWnd, ID_LISTVIEW);

	addListViewItem(listview, x1, y1, x2, y2);

	Cut *cut = newCutInt(x1, y1, x2, y2);
	return addToTable(table, cut);
}

void updateColumn(int col, LV_ITEM lvi, 
	HWND listview, HWND edit, int &save)
{
	TCHAR buf[BUFFER_MAX_SIZE];
	lvi.iSubItem = col;
	lvi.pszText = buf;
	lvi.cchTextMax = BUFFER_MAX_SIZE - 1;
	ListView_GetItem(listview, &lvi);
	save = atoi(lvi.pszText); //listview_x
	setCharText(edit, lvi.pszText); //edit_x
}

void updateRow(int index, HWND hWnd,
	int &x1, int &y1,
	int &x2, int &y2)
{
	LV_ITEM lvi;
	memset(&lvi, 0, sizeof(lvi));
	lvi.mask = LVIF_IMAGE | LVIF_INDENT |
		LVIF_PARAM | LVIF_STATE | LVIF_TEXT;
	lvi.iItem = index;

	HWND listview = GetDlgItem(hWnd, ID_LISTVIEW);

	updateColumn(0, lvi, listview,
		GetDlgItem(hWnd, ID_EDIT_X1), x1);

	updateColumn(1, lvi, listview,
		GetDlgItem(hWnd, ID_EDIT_Y1), y2);

	updateColumn(2, lvi, listview,
		GetDlgItem(hWnd, ID_EDIT_X2), x2);

	updateColumn(3, lvi, listview,
		GetDlgItem(hWnd, ID_EDIT_Y2), y2);
}

void updatingListView(HWND hWnd, LPARAM lParam, 
	int &listview_choosen, int &x1,
	int &y1, int &x2, int &y2)
{
	HWND listview = GetDlgItem(hWnd, ID_LISTVIEW);

	EnableWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), TRUE);
	EnableWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), TRUE);

	NMLISTVIEW * plv;
	plv = (NMLISTVIEW*)lParam;
	plv->iItem;

	int index = ListView_GetNextItem(listview,
		-1, LVNI_ALL | LVNI_SELECTED);

	if (index == -1)
		return;
	listview_choosen = index;

	updateRow(index, hWnd, x1, y1, x2, y2);

	HDC hdc = GetDC(hWnd);
	drawEllipse(hdc, x1, y1, 4, RGB(0, 0, 0));
	drawEllipse(hdc, x2, y2, 3, RGB(200, 200, 200));
	ReleaseDC(hWnd, hdc);
}

void deleting(HWND hWnd, int listview_choosen,
	int x1, int y1, int x2, int y2, Table* table)
{
	deleteListViewItem(GetDlgItem(hWnd, ID_LISTVIEW),
		listview_choosen);

	Cut *cut = newCutInt(x1, y1, x2, y2);
	table = deleteOfTable(table, cut);
	deleteCut(&cut);

	EnableWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), FALSE);
	EnableWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), FALSE);
}

void changing(HWND hWnd, int listview_choosen,
	Table* table, int old_x1, int old_y1,
	int old_x2, int old_y2)
{
	HWND listview = GetDlgItem(hWnd, ID_LISTVIEW);
	deleteListViewItem(listview, listview_choosen);
	
	int x1 = getIntText(hWnd, ID_EDIT_X1);
	int y1 = getIntText(hWnd, ID_EDIT_Y1);
	int x2 = getIntText(hWnd, ID_EDIT_X2);
	int y2 = getIntText(hWnd, ID_EDIT_Y2);

	addListViewItem(listview, x1, y1, x2, y2);

	Cut *oldCut = newCutInt(old_x1, old_y1, old_x2, old_y2);
	Cut *newCut = newCutInt(x1, y1, x2, y2);

	changeTable(table, oldCut, newCut);

	deleteCut(&oldCut);
	deleteCut(&newCut);

	EnableWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), FALSE);
	EnableWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), FALSE);
}
