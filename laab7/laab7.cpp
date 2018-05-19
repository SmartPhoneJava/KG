// WindowsProject1.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "laab7.h"
#include <windows.h>
#include <conio.h>
#include <string>
#include <iostream>
#include <CommCtrl.h>
#include <tchar.h>
#include <ctime>

using namespace std;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

#define ID_BUTTON_ADD 1
#define ID_BUTTON_DELETE 2
#define ID_BUTTON_CLEAR 3
#define ID_BUTTON_ANSWER 4
#define ID_BUTTON_SAVE 5
#define ID_BUTTON_BUILD 6
#define ID_BUTTON_CHANGE 7
#define ID_BUTTON_LOAD 9

#define ID_EDIT_I 10
#define ID_EDIT_X 11
#define ID_EDIT_Y 12
#define ID_NEDIT_X 13
#define ID_NEDIT_Y 14
#define ID_NEDIT_I 15

#define ID_EDIT_ZX 16
#define ID_EDIT_ZY 17
#define ID_NEDIT_ZX 18
#define ID_NEDIT_ZY 19

#define ID_ZEDIT_ZX 21
#define ID_ZEDIT_ZY 22
#define ID_ZNEDIT_ZX 23
#define ID_ZNEDIT_ZY 24
#define ID_ZTEXT 25

#define ID_LISTVIEW 20

#define BUFFER_MAX_SIZE 64

#define MAX_LOADSTRING 100

#define LEFT 140
#define RIGHT 150

#define NO_ERROR 0
#define ERROR_NO_FIRST_TOP -1
#define ERROR_NO_SECOND_TOP -2
#define ERROR_NOT_ENOUGH_POINTS_FIRST -3
#define ERROR_NOT_ENOUGH_POINTS_SECOND -4

#define ERROR_BORDERS -5
#define ERROR_NULL_POINTER -6

#include "Point.h"
#include "Cut.h"
#include "Debug.h"
#include "Alg.h"
#include "Secatel.h"
#include <iostream>

typedef struct Row
{
	double X;
	double Y;
	int i;
	struct Row* next;
} Row;

typedef struct Table
{
	Row* type;
	int max;
} Table;

typedef struct Rebro
{
	int x;
	int y;
	double dx;
	int dy;
	Rebro* next;
} Rebro;

typedef struct Figure
{
	Rebro* rebro;
} Figure;

typedef struct Stroka
{
	int x;
	int y;
	int type;
	Stroka* next;
} Stroka;

typedef struct Stack
{
	Point* point;
	Stack* next;
} Stack;

void fill(HDC hdc, int zx, int zy);

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

int listview_choosen = 0;
int listview_x = 0;
int listview_y = 0;
int listview_i = 0;
int our_i = 1;

void drawCutInside(HWND hWnd, Cut cut,
	Secatel sec);

static double XStartPos = 100, YStartPos = 20, WidthGrid = 400, HeightGrid = 400, StepGrid = 2, Radius = 4;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

Table *table;

Table* init_table();

Table* add_to_list(Table *table, double x, double y, int i);

void bigAdd(HWND hWnd);

Table* free_memory(Table *tab);

void addListViewItem(HWND ListView, int row, int x, int y, int i);

bool isStringNumber(string s);

void getLastXY(int *x, int *y);

void changeElement(int find_x, int find_y, int find_i,
	int new_x, int new_y, int new_i);

void setCharText(HWND hwnd, char* text);

void drawLines(HWND hwnd);

int getTableSize(Table *table);

void peresech(HWND hWnd, Stroka **original, Rebro *figure, int y, int i);

void DrawLine(HDC hdc, int X1, int Y1, int X2, int Y2);

void greatRewrite(HWND hWnd, int with_clean);

void drawPoint(HWND hWnd, double XBuf, double YBuf, int radius, int r, int g, int b);

void drawPoints(HWND hWnd, int with);

Row* search_in_list(Table *table, double x, double y, int i);

void cleanRect(HWND hWnd, LONG left, LONG top, LONG right, LONG bottom);

Table* delete_from_list(Table *table, double x, double y, int type);

void drawRowLine(HWND hWnd, Row* row1, Row* row2, int type);

void drawFigure(HWND hWnd);

void printError(HWND hWnd, int number);

void drawEllipseBr(HDC hdc, int cx, int cy, int rx, int ry, DWORD cur);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAAB7, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAAB7));

	MSG msg;

	Table *table = init_table();

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

void debugDouble(const char* text, double number)
{
	char str[BUFFER_MAX_SIZE];
	sprintf_s(str, "%s: %f \n", text, number);

	OutputDebugString(str);
}

void debug(const char* text, int number)
{
	char str[BUFFER_MAX_SIZE];
	sprintf_s(str, "%s: %d \n", text, number);

	OutputDebugString(str);
}

void debugStroka(const char* text, Stroka *str)
{
	Stroka *s = str;
	if (str->y >= 0)
	{
		debug("y", str->y);
		while (s != NULL)
		{
			debug("x", str->x);
			s = s->next;
		}
	}
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_LAAB7));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAAB7);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR text_answer_text[BUFFER_MAX_SIZE] = " ";
	TCHAR text_edit[BUFFER_MAX_SIZE] = "edit";
	TCHAR text_button[BUFFER_MAX_SIZE] = "button";
	TCHAR text_add[BUFFER_MAX_SIZE] = "Добавить";
	TCHAR text_delete[BUFFER_MAX_SIZE] = "Удалить";
	TCHAR text_save[BUFFER_MAX_SIZE] = "Сохранить";
	TCHAR text_clear[BUFFER_MAX_SIZE] = "Очистить";
	TCHAR text_build[BUFFER_MAX_SIZE] = "Построить";
	TCHAR text_cancel[BUFFER_MAX_SIZE] = "Назад";
	TCHAR text_load[BUFFER_MAX_SIZE] = "Пример";
	TCHAR text_change[BUFFER_MAX_SIZE] = "Изменить";
	TCHAR text_0[BUFFER_MAX_SIZE] = "0";
	TCHAR text_1[BUFFER_MAX_SIZE] = "1";
	TCHAR text_2[BUFFER_MAX_SIZE] = "2";
	TCHAR text_x[BUFFER_MAX_SIZE] = "X";
	TCHAR text_y[BUFFER_MAX_SIZE] = "Y";
	TCHAR text_p[BUFFER_MAX_SIZE] = "Полигон";
	TCHAR text_type[BUFFER_MAX_SIZE] = "Множество";
	TCHAR text_typ[BUFFER_MAX_SIZE] = "Тип";

	COLOR16 color_white = 0xFFFFFF;
	COLOR16 color_blue = RGB(106, 90, 205);
	COLOR16 color_darkblue = RGB(72, 61, 139);
	COLOR16 color_red = RGB(220, 20, 60);
	COLOR16 color_darkred = RGB(139, 0, 0);
	COLOR16 color_middlered = RGB(200, 0, 0);
	static bool clicked = FALSE;
	static bool ctrl = FALSE;
	static bool shift = FALSE;
	static bool sec = FALSE;
	static int zx = 100, zy = 100, x1, x2, y1, y2;

	switch (message)
	{
	case WM_KEYDOWN:
	{
		if (wParam == 16)
		{
			shift = TRUE;
		}
		else if (wParam == 17)
		{
			ctrl = TRUE;
		}
		break;
	}
	case WM_KEYUP:
	{
		if (wParam == 16)
		{
			shift = FALSE;
		}
		else if (wParam == 17)
		{
			ctrl = FALSE;
		}
		break;
	}
	case WM_CREATE:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		CreateWindow(text_button, text_add, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, // | BS_OWNERDRAW,
			rc.right - 300, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_ADD, NULL, NULL);
		CreateWindow(text_button, text_delete, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, // | BS_OWNERDRAW,
			rc.right - 200, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_DELETE, NULL, NULL);
		CreateWindow(text_button, text_build, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, // | BS_OWNERDRAW,
			rc.right - 200, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_BUILD, NULL, NULL);
		CreateWindow(text_button, text_clear, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, // | BS_OWNERDRAW,
			rc.right - 200, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_CLEAR, NULL, NULL);
		CreateWindow(text_button, text_save, WS_CHILD | BS_PUSHBUTTON,
			rc.right - 200, 2, 100, 20, hWnd, (HMENU)ID_BUTTON_SAVE, NULL, NULL);
		CreateWindow(text_button, text_clear, WS_CHILD | BS_PUSHBUTTON,
			rc.right - 200, 22, 100, 20, hWnd, (HMENU)ID_BUTTON_CLEAR, NULL, NULL);
		CreateWindow(text_button, text_change, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			rc.right - 100, 2, 98, 20, hWnd, (HMENU)ID_BUTTON_CHANGE, NULL, NULL);
		CreateWindow(text_edit, text_0, WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.right - 280, 50, 170, 20, hWnd, (HMENU)ID_EDIT_X, NULL, NULL);
		CreateWindow(text_edit, text_x, WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.right - 300, 50, 15, 20, hWnd, (HMENU)ID_NEDIT_X, NULL, NULL);
		CreateWindow(text_edit, text_0, WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.right - 280, 75, 170, 20, hWnd, (HMENU)ID_EDIT_Y, NULL, NULL);
		CreateWindow(text_edit, text_y, WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.right - 300, 75, 15, 20, hWnd, (HMENU)ID_NEDIT_Y, NULL, NULL);
		CreateWindow(text_edit, text_1, WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.right - 125, 75, 170, 20, hWnd, (HMENU)ID_EDIT_I, NULL, NULL);
		CreateWindow(text_edit, text_p, WS_CHILD | WS_VISIBLE,
			rc.right - 110, 50, 170, 20, hWnd, (HMENU)ID_NEDIT_I, NULL, NULL);

		CreateWindow(text_edit, "Затравочный пиксель", WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.right - 300, 100, 300, rc.bottom - 100, hWnd, (HMENU)ID_ZTEXT, NULL, NULL);

		CreateWindow(text_edit, "100", WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.right - 300, 20, 300, rc.bottom - 100, hWnd, (HMENU)ID_ZEDIT_ZX, NULL, NULL);

		CreateWindow(text_edit, text_x, WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.right - 300, 20, 300, rc.bottom - 100, hWnd, (HMENU)ID_ZNEDIT_ZX, NULL, NULL);

		CreateWindow(text_edit, "100", WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.right - 300, 20, 300, rc.bottom - 100, hWnd, (HMENU)ID_ZEDIT_ZY, NULL, NULL);

		CreateWindow(text_edit, text_y, WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.right - 300, 20, 300, rc.bottom - 100, hWnd, (HMENU)ID_ZNEDIT_ZY, NULL, NULL);

		CreateWindow(WC_LISTVIEW, text_1, WS_CHILD | LVS_REPORT | LVS_EX_FULLROWSELECT | WS_VSCROLL | WS_VISIBLE,
			rc.right - 300, 125, 300, rc.bottom - 100, hWnd, (HMENU)ID_LISTVIEW, NULL, NULL);

		EnableWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), FALSE);
		EnableWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), FALSE);
		EnableWindow(GetDlgItem(hWnd, ID_NEDIT_I), FALSE);
		ListView_SetExtendedListViewStyle(GetDlgItem(hWnd, ID_LISTVIEW), LVS_EX_FULLROWSELECT);

		LV_COLUMN lc; lc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
		lc.fmt = LVCFMT_CENTER;
		lc.pszText = text_x; lc.iSubItem = 0; lc.cx = 40;
		SendDlgItemMessage(hWnd, ID_LISTVIEW, LVM_INSERTCOLUMN, 0, (LPARAM)&lc);
		lc.pszText = text_y; lc.iSubItem = 1; lc.cx = 40;
		SendDlgItemMessage(hWnd, ID_LISTVIEW, LVM_INSERTCOLUMN, 1, (LPARAM)&lc);
		lc.pszText = text_typ; lc.iSubItem = 2; lc.cx = 40;
		SendDlgItemMessage(hWnd, ID_LISTVIEW, LVM_INSERTCOLUMN, 2, (LPARAM)&lc);

		ListView_SetColumnWidth(GetDlgItem(hWnd, ID_LISTVIEW), 0, 130);
		ListView_SetColumnWidth(GetDlgItem(hWnd, ID_LISTVIEW), 1, 130);
		ListView_SetColumnWidth(GetDlgItem(hWnd, ID_LISTVIEW), 2, 40);

		SendDlgItemMessage(hWnd, ID_NEDIT_X, EM_SETREADONLY, 1, 0);
		SendDlgItemMessage(hWnd, ID_NEDIT_Y, EM_SETREADONLY, 1, 0);

		table = init_table();

		return 0;
	}
	case WM_RBUTTONDOWN:
		if (!ctrl)
		{
			our_i++;

			TCHAR buffer2[BUFFER_MAX_SIZE];
			_snprintf_s(buffer2, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", our_i);

			setCharText(GetDlgItem(hWnd, ID_EDIT_I), buffer2);
		}
		else
		{
			int mouse_click_x = LOWORD(lParam);
			if (mouse_click_x > 699)
				break;
			int mouse_click_y = HIWORD(lParam);
			debug("mouse_click_x", mouse_click_x);
			debug("mouse_click_y", mouse_click_y);
			if (!sec)
			{
				x1 = mouse_click_x;
				y1 = mouse_click_y;
			}
			else
			{
				x2 = mouse_click_x;
				y2 = mouse_click_y;
				HDC hDc = GetDC(hWnd);
				Ellipse(hDc, x1, y1, x2, y2);
				ReleaseDC(hWnd, hDc);
				debug("here", mouse_click_x);
			}
			sec = !sec;
		}

		break;
	case WM_LBUTTONUP:
	{
		clicked = FALSE;
		break;
	}
	case WM_LBUTTONDOWN:
	{
		clicked = TRUE;
		int mouse_click_x = LOWORD(lParam);
		if (mouse_click_x > 699)
			break;
		int mouse_click_y = HIWORD(lParam);
	}
	case WM_MOUSEMOVE:
	{
		if (!clicked)
			break;
		int mouse_click_x = LOWORD(lParam);
		if (mouse_click_x > 699)
			break;
		int mouse_click_y = HIWORD(lParam);
		if (!shift)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);

			int x = 0, y = 0;
			getLastXY(&x, &y);
			if (ctrl)
			{
				if (abs(x - mouse_click_x) > abs(y - mouse_click_y))
					mouse_click_y = y;
				else
					mouse_click_x = x;
			}

			TCHAR buffer[BUFFER_MAX_SIZE];
			_snprintf_s(buffer, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", mouse_click_x);

			TCHAR buffer1[BUFFER_MAX_SIZE];
			_snprintf_s(buffer1, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", mouse_click_y);

			setCharText(GetDlgItem(hWnd, ID_EDIT_X), buffer);
			setCharText(GetDlgItem(hWnd, ID_EDIT_Y), buffer1);

			table = add_to_list(table, mouse_click_x, mouse_click_y, our_i);
			drawLines(hWnd);
			table = delete_from_list(table, mouse_click_x, mouse_click_y, our_i);
		}
		else
		{
			TCHAR buffer[BUFFER_MAX_SIZE];
			_snprintf_s(buffer, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", mouse_click_x);

			TCHAR buffer1[BUFFER_MAX_SIZE];
			_snprintf_s(buffer1, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", mouse_click_y);

			setCharText(GetDlgItem(hWnd, ID_ZEDIT_ZX), buffer);
			setCharText(GetDlgItem(hWnd, ID_ZEDIT_ZY), buffer1);

			zx = mouse_click_x;
			zy = mouse_click_y;

			drawLines(hWnd);
		}
		drawPoint(hWnd, zx, zy, 5, 0, 0, 255);
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		//drawPoints(hWnd, 1);
		EndPaint(hWnd, &ps);
		break;
	}

	//https://stackoverflow.com/questions/18745447/how-can-i-change-the-background-color-of-a-button-winapi-c
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT Item;
		Item = (LPDRAWITEMSTRUCT)lParam;
		SelectObject(Item->hDC, CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial Black"));
		SelectObject(Item->hDC, CreateSolidBrush(0));

		if ((getTableSize(table) == 0))
		{
			if (Item->itemState & ODS_SELECTED)
			{
				FillRect(Item->hDC, &Item->rcItem, CreateSolidBrush(color_darkred));
				SetTextColor(Item->hDC, 0xEEEEEE);
				SelectObject(Item->hDC, CreateSolidBrush(color_darkred));
				SelectObject(Item->hDC, CreatePen(PS_SOLID, 2, color_darkred));
			}
			else
			{
				FillRect(Item->hDC, &Item->rcItem, CreateSolidBrush(color_red));
				SetTextColor(Item->hDC, 0xFFFFFF);
				SelectObject(Item->hDC, CreateSolidBrush(color_red));
				SelectObject(Item->hDC, CreatePen(PS_SOLID, 2, color_red));

			}
		}
		else
		{
			if (Item->itemState & ODS_SELECTED)
			{
				FillRect(Item->hDC, &Item->rcItem, CreateSolidBrush(RGB(72, 61, 139)));
				SetTextColor(Item->hDC, 0xEEEEEE);
				SelectObject(Item->hDC, CreateSolidBrush(RGB(72, 61, 139)));
				SelectObject(Item->hDC, CreatePen(PS_SOLID, 2, RGB(72, 61, 139)));
			}
			else
			{
				FillRect(Item->hDC, &Item->rcItem, CreateSolidBrush(RGB(106, 90, 205)));
				SetTextColor(Item->hDC, 0xFFFFFF);
				SelectObject(Item->hDC, CreateSolidBrush(RGB(106, 90, 205)));
				SelectObject(Item->hDC, CreatePen(PS_SOLID, 2, RGB(106, 90, 205)));

			}
		}
		SetBkMode(Item->hDC, TRANSPARENT);
		RoundRect(Item->hDC, Item->rcItem.left, Item->rcItem.top, Item->rcItem.right, Item->rcItem.bottom, 20, 20);
		int len;
		len = GetWindowTextLength(Item->hwndItem);
		LPSTR lpBuff;
		lpBuff = new char[len + 1];
		GetWindowTextA(Item->hwndItem, lpBuff, len + 1);
		DrawTextA(Item->hDC, lpBuff, len, &Item->rcItem, DT_CENTER);
		break;
	}
	// перемещение окна
	case WM_SIZE:
	{
		greatRewrite(hWnd, 0);

		break;
	}
	// http://www.cyberforum.ru/win-api/thread1098433.html
	case WM_NOTIFY:
	{
		HWND edit_x = GetDlgItem(hWnd, ID_EDIT_X);
		HWND edit_i = GetDlgItem(hWnd, ID_EDIT_I);
		HWND edit_y = GetDlgItem(hWnd, ID_EDIT_Y);
		HWND nedit_x = GetDlgItem(hWnd, ID_NEDIT_X);
		HWND nedit_y = GetDlgItem(hWnd, ID_NEDIT_Y);
		HWND nedit_i = GetDlgItem(hWnd, ID_NEDIT_I);
		HWND listview = GetDlgItem(hWnd, ID_LISTVIEW);
		HWND button_save = GetDlgItem(hWnd, ID_BUTTON_SAVE);

		switch (((LPNMHDR)lParam)->code)
		{
		case LVN_ITEMCHANGED:
		{
			TCHAR buffer[BUFFER_MAX_SIZE];
			GetWindowTextA(button_save, (LPSTR)buffer, sizeof(buffer));
			if (strcmp(buffer, "Добавить") == 0)
			{
				break;
			}

			EnableWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), TRUE);
			EnableWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), TRUE);

			NMLISTVIEW * plv;
			plv = (NMLISTVIEW*)lParam;
			plv->iItem;

			TCHAR szBuf[BUFFER_MAX_SIZE], buf[BUFFER_MAX_SIZE];

			lstrcpy(szBuf, TEXT("Выбран элемент:\n"));
			int index = ListView_GetNextItem(listview, -1, LVNI_ALL | LVNI_SELECTED);

			if (index == -1)
			{
				return 0;
			}
			listview_choosen = index;

			LV_ITEM lvi;
			memset(&lvi, 0, sizeof(lvi));
			lvi.mask = LVIF_IMAGE | LVIF_INDENT | LVIF_PARAM | LVIF_STATE | LVIF_TEXT;

			lvi.iItem = index;
			lvi.iSubItem = 0;
			lvi.pszText = buf;
			lvi.cchTextMax = BUFFER_MAX_SIZE - 1;
			ListView_GetItem(listview, &lvi);
			lstrcat(szBuf, lvi.pszText);
			listview_x = atoi(lvi.pszText);
			setCharText(edit_x, lvi.pszText);

			lvi.iSubItem = 1;
			lvi.pszText = buf;
			lvi.cchTextMax = BUFFER_MAX_SIZE - 1;
			ListView_GetItem(listview, &lvi);
			listview_y = atoi(lvi.pszText);
			setCharText(edit_y, lvi.pszText);

			lvi.iSubItem = 2;
			lvi.pszText = buf;
			lvi.cchTextMax = BUFFER_MAX_SIZE - 1;
			ListView_GetItem(listview, &lvi);
			listview_i = atoi(lvi.pszText);
			setCharText(edit_i, lvi.pszText);

			drawPoints(hWnd, 1);

			drawPoint(hWnd, listview_x, listview_y, Radius * 2, 0, 0, 0);
			drawPoint(hWnd, listview_x, listview_y, Radius * 1.5, 200, 200, 200);

			Row *row = search_in_list(table, listview_x, listview_y, listview_i);
			if (row == NULL)
			{
				MessageBox(hWnd, "Cant found point", "InsideError", MB_OK);
				break;
			}
			break;
		}
		}
	}
	case WM_COMMAND:
	{
		HWND button_delete = GetDlgItem(hWnd, ID_BUTTON_DELETE);
		HWND button_add = GetDlgItem(hWnd, ID_BUTTON_ADD);
		HWND button_save = GetDlgItem(hWnd, ID_BUTTON_SAVE);
		HWND button_change = GetDlgItem(hWnd, ID_BUTTON_CHANGE);
		HWND button_answer = GetDlgItem(hWnd, ID_BUTTON_ANSWER);
		HWND button_clear = GetDlgItem(hWnd, ID_BUTTON_CLEAR);
		HWND button_load = GetDlgItem(hWnd, ID_BUTTON_LOAD);
		HWND edit_i = GetDlgItem(hWnd, ID_EDIT_I);
		HWND edit_x = GetDlgItem(hWnd, ID_EDIT_X);
		HWND edit_y = GetDlgItem(hWnd, ID_EDIT_Y);
		HWND nedit_x = GetDlgItem(hWnd, ID_NEDIT_X);
		HWND nedit_y = GetDlgItem(hWnd, ID_NEDIT_Y);
		HWND nedit_i = GetDlgItem(hWnd, ID_NEDIT_I);
		HWND listview = GetDlgItem(hWnd, ID_LISTVIEW);

		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case ID_BUTTON_BUILD:
		{
			Point *p1 = newPoint(50, 50);
			Point *p2 = newPoint(400, 200);
			Cut *cut = newCut(*p1, *p2);

			Secatel *sec = newSecatel(100,
				300,180,100);

			drawCutInside(hWnd, *cut, *sec);
			//drawFigure(hWnd);
			break;
		}
		case ID_BUTTON_DELETE:
		{
			if (getTableSize(table) == 0)
			{
				MessageBox(NULL, "There is no any point. You cant delete anything!", "Warning", MB_OK);
			}
			else
			{
				ListView_DeleteItem(listview, listview_choosen);

				table = delete_from_list(table, listview_x, listview_y, listview_i);

				EnableWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), FALSE);
				EnableWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), FALSE);
				drawPoints(hWnd, 1);
			}
			break;
		}
		case ID_BUTTON_CHANGE:
		{
			if (IsWindowVisible(button_change))
			{
				if (getTableSize(table) == 0)
				{
					MessageBox(NULL, "There is no any point. You cant change anything!", "Warning", MB_OK);
				}
				else
				{
					ListView_DeleteItem(listview, listview_choosen);
					TCHAR buffer[BUFFER_MAX_SIZE];
					GetWindowText(edit_x, buffer, sizeof(buffer));

					TCHAR buffer1[BUFFER_MAX_SIZE];
					GetWindowText(edit_y, buffer1, sizeof(buffer1));

					TCHAR buffer2[BUFFER_MAX_SIZE];
					GetWindowText(edit_i, buffer2, sizeof(buffer2));

					if (!isStringNumber(buffer))
					{
						MessageBox(NULL, "X - wrong symbols. Use only numbers,point and minus", "IOError", MB_OK);
						break;
					}

					if (!isStringNumber(buffer1))
					{
						MessageBox(NULL, "Y - wrong symbols. Use only numbers,point and minus", "IOError", MB_OK);
						break;
					}

					//table = delete_from_list(table, listview_x, listview_y, listview_i);

					int value_x = atoi(buffer);
					int value_y = atoi(buffer1);
					int value_i = atoi(buffer2);

					addListViewItem(listview, 0, value_x, value_y, value_i);
					//table = add_to_list(table, value_x, value_y, value_i);

					changeElement(listview_x, listview_y, listview_i,
						value_x, value_y, value_i);

					EnableWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), FALSE);
					EnableWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), FALSE);

					drawPoints(hWnd, 1);
				}
			}
			break;
		}
		case ID_BUTTON_CLEAR:
		{
			if (IsWindowVisible(button_clear))
			{
				Row *row;
				table = free_memory(table);

				ListView_DeleteAllItems(listview);
			}
			break;
		}
		case ID_BUTTON_ADD:
		{
			bigAdd(hWnd);
			drawPoints(hWnd, 1);
			drawPoint(hWnd, zx, zy, 5, 0, 0, 255);
			break;
		}
		case IDM_ABOUT:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		}
		case IDM_EXIT:
		{
			DestroyWindow(hWnd);
			break;
		}
		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		}
		SetFocus(hWnd);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Перерисовка экрана
void greatRewrite(HWND hWnd, int with_clean)
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	if (with_clean)
	{
		cleanRect(hWnd, 0, 0, 300, 0);
	}

	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_ADD), rc.right - 300, 2, 100, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), rc.right - 200, 2, 100, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_SAVE), rc.right - 200, 2, 100, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_BUILD), rc.right - 300, 22, 100, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_CLEAR), rc.right - 200, 22, 100, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_ANSWER), rc.right - 300, 22, 148, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), rc.right - 100, 2, 98, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_NEDIT_I), rc.right - 100, 50, 89, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_EDIT_I), rc.right - 100, 75, 89, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_NEDIT_X), rc.right - 300, 50, 15, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_EDIT_X), rc.right - 280, 50, 170, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_EDIT_Y), rc.right - 280, 75, 170, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_NEDIT_Y), rc.right - 300, 75, 15, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_ZTEXT), rc.right - 300, 100, 150, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_ZNEDIT_ZX), rc.right - 300, 120, 15, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_ZEDIT_ZX), rc.right - 280, 120, 50, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_ZEDIT_ZY), rc.right - 200, 120, 50, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_ZNEDIT_ZY), rc.right - 220, 120, 15, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_LISTVIEW), rc.right - 300, 140, 300, rc.bottom - 100, 0);

	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_LOAD), rc.right - 150, 22, 148, 20, 0);

}

void getLastXY(int *x, int *y)
{
	*x = 0, *y = 0;
	Row *row = table->type;
	if (row)
	{
		for (int i = 1; i <= table->max; i++)
		{
			row = table->type;
			while (row != NULL)
			{
				if (row->i == i)
				{
					*x = row->X;
					*y = row->Y;
				}
				row = row->next;
			}
		}

	}
}

void changeElement(int find_x, int find_y, int find_i,
	int new_x, int new_y, int new_i)
{
	int x = 0, y = 0;
	Row *row = table->type;
	if (row)
	{
		for (int i = 1; i <= table->max; i++)
		{
			row = table->type;
			while (row != NULL)
			{
				if ((row->i == find_i) && (row->X == find_x) && (row->Y == find_y))
				{
					row->X = new_x;
					row->Y = new_y;
					row->i = new_i;
				}
				row = row->next;
			}
		}

	}
}

void bigAdd(HWND hWnd)
{
	HWND edit_x = GetDlgItem(hWnd, ID_EDIT_X);
	HWND edit_i = GetDlgItem(hWnd, ID_EDIT_I);
	HWND edit_y = GetDlgItem(hWnd, ID_EDIT_Y);
	HWND listview = GetDlgItem(hWnd, ID_LISTVIEW);

	TCHAR buffer[BUFFER_MAX_SIZE];
	GetWindowText(edit_x, buffer, sizeof(buffer));

	TCHAR buffer1[BUFFER_MAX_SIZE];
	GetWindowText(edit_y, buffer1, sizeof(buffer1));

	TCHAR buffer2[BUFFER_MAX_SIZE];
	GetWindowText(edit_i, buffer2, sizeof(buffer2));

	if (!isStringNumber(buffer))
	{
		MessageBox(NULL, "X - wrong symbols. Use only numbers,point and minus", "IOError", MB_OK);
		return;
	}

	if (!isStringNumber(buffer1))
	{
		MessageBox(NULL, "Y - wrong symbols. Use only numbers,point and minus", "IOError", MB_OK);
		return;
	}

	int value_x = atoi(buffer);
	int value_y = atoi(buffer1);
	int value_i = atoi(buffer2);

	addListViewItem(listview, 0, value_x, value_y, value_i);
	table = add_to_list(table, value_x, value_y, value_i);
}

// Печать сообщения об ошибке
void printError(HWND hWnd, int number)
{
	switch (number)
	{
	case ERROR_NOT_ENOUGH_POINTS_FIRST:
	{
		MessageBox(hWnd, "Not enough points of first type", "UserError", MB_OK);
		break;
	}
	case ERROR_NOT_ENOUGH_POINTS_SECOND:
	{
		MessageBox(hWnd, "Not enough points of second type", "UserError", MB_OK);
		break;
	}
	case ERROR_NO_FIRST_TOP:
	{
		MessageBox(hWnd, "No top of first type", "UserError", MB_OK);
		break;
	}
	case ERROR_NO_SECOND_TOP:
	{
		MessageBox(hWnd, "No top of second type", "UserError", MB_OK);
		break;
	}
	}
}

// Инициализация структуры
Table* init_table()
{
	Table *table = (Table*)(calloc(1, 2 * sizeof(Row*)));
	table->type = NULL;
	table->max = 0;

	return table;
}

// Добавить в список
Table* add_to_list(Table *table, double x, double y, int i)
{
	Row *row = table->type, *row_prev = NULL;

	int i_max = 0;
	while (row != NULL)
	{
		row_prev = row;
		if (row->i > i_max)
			i_max = row->i;
		row = row->next;
	}

	row = (Row*)(calloc(1, 2 * sizeof(double) + sizeof(int)));
	row->X = x;
	row->Y = y;
	row->i = i;

	if (i > i_max)
		i_max = i;

	row->next = NULL;
	if (row_prev)
	{
		row_prev->next = row;
	}
	else
	{
		table->type = row;
	}
	table->max = i_max;
	return table;
}

Table* delete_from_list(Table *table, double x, double y, int i)
{
	Row *row = table->type, *row_prev = NULL;

	while (row != NULL)
	{
		if ((row->X == x) && (row->Y == y) && (row->i == i))
		{
			break;
		}
		row_prev = row;
		row = row->next;
	}

	if (row_prev)
	{
		if (row)
		{
			row_prev->next = row->next;
		}
		else
		{
			row_prev->next = NULL;
		}
	}
	else
	{
		table->type = row->next;
	}
	//free(row);
	return table;
}

// Поиск в списке по данным точки
Row* search_in_list(Table *table, double x, double y, int i)
{
	Row *row = table->type;
	while (row != NULL)
	{
		if ((row->X == x) && (row->Y == y) && (row->i == i))
		{
			break;
		}
		row = row->next;
	}
	return row;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
int getSize(Stroka *k)
{
	Stroka* str = k;
	int size = 0;
	while (str != NULL)
	{
		size++;
		str = str->next;
	}
	return size;
}

// Нарисовать прямую по заданной линии
void drawRowLine(HWND hWnd, Row* row1, Row* row2, int type)
{
	HDC hdc = GetDC(hWnd);

	double x1, y1, x2, y2;

	x1 = row1->X;
	y1 = row1->Y;
	x2 = row2->X;
	y2 = row2->Y;

	HPEN p1, p2;
	if (type == 1)
	{
		p2 = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	}
	else if (type == 2)
	{
		p2 = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	}
	else if (type == 3)
	{
		p2 = CreatePen(PS_SOLID, 2, RGB(255, 0, 255));
	}
	else
	{
		MessageBox(hWnd, "Wrong type in drawRowLine()", "Warning", MB_OK);
		p2 = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	}
	p1 = (HPEN)SelectObject(hdc, p2);  // Заносим красное перо в контекст рисования
	MoveToEx(hdc, (int)x1, (int)y1, NULL);
	LineTo(hdc, (int)x2, (int)y2);    // Проводим линию
	SelectObject(hdc, p1);   // Возвращаем старое перо
	DeleteObject(p2);       // Удаляем созданное перо
}

Stroka *makeStr(int x, int y, int type)
{
	Stroka *str = (Stroka*)malloc(sizeof(Stroka));
	str->x = x;
	str->y = y;
	str->type = type;
	str->next = NULL;
	return str;
}

// Очистить область
void cleanRect(HWND hWnd, LONG left, LONG top, LONG right, LONG bottom)
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
}

int getMiny(Rebro *rebro)
{
	if (!rebro)
		return 0;
	Rebro* move = rebro;
	int y2 = 0;
	while (move != NULL)
	{
		y2 = move->y - move->dy;
		move = move->next;
	}
	return y2;
}

bool nea(int a, int b)
{
	return (abs(a - b) < 2);
}

int borderFigure(HDC hdc, Stroka *display, int minY, int maxY, int displaySize)
{
	//if ((displaySize > maxY) || (minY < 0))
	//	return ERROR_BORDERS;

	Stroka *s;
	for (int y = minY; y < maxY; y++)
	{
		s = &(display[y]);
		while ((s != NULL) && (s->next != NULL))
		{
			if (s->next->type == s->type)
			{
				if (!nea(s->x, s->next->x))
				{
					SetPixel(hdc, s->x, y, RGB(0, 0, 0));
					SetPixel(hdc, s->next->x, y, RGB(0, 0, 0));
				}
				else
				{
					SetPixel(hdc, s->x, y, RGB(0, 0, 0));
					SetPixel(hdc, s->x + 1, y, RGB(0, 0, 0));
				}
			}
			s = s->next;
		}
	}
	return NOERROR;
}

DWORD get_pixel_color(HBITMAP bmp, int x, int y)
{
	WORD _temp;
	char *pixel;
	BITMAP bmInfo;
	DWORD pixel_color;
	GetObject(bmp, sizeof(BITMAP), &bmInfo);
	pixel = (char*)bmInfo.bmBits;
	pixel += bmInfo.bmWidthBytes*y;
	pixel += x * bmInfo.bmBitsPixel;
	if (!(DWORD*)pixel)
		return -1;
	switch (bmInfo.bmBitsPixel)
	{
	case 16:_temp = *(WORD*)pixel;
		pixel_color = _temp & 0x1F;
		_temp >>= 5;
		pixel_color = (_temp & 0x3F) << 8;
		_temp >>= 6;
		pixel_color = (_temp & 0x1F) << 0x10;
		break;
	case 24:pixel_color = *(DWORD*)pixel & 0xFFFFFF;
		break;
	case 32:pixel_color = *(DWORD*)pixel;
		break;
	}
	return pixel_color;
}

void drawLines(HWND hwnd)
{
	cleanRect(hwnd, 0, 0, 300, 0);
	Row *row = table->type, *last = NULL;
	int j = 0;
	int k = 0;
	HDC hdc = GetDC(hwnd);
	if (row)
	{
		int cx = 0, cy = 0;
		int lx = 0, ly = 0;
		int x = 0, y = 0;
		for (int i = 1; i <= table->max; i++)
		{
			j = 0;
			k = 0;
			last = NULL;
			row = table->type;
			while (row != NULL)
			{
				if (row->i == i)
				{
					x = row->X;
					y = row->Y;

					if (!j)
					{
						cx = x;
						cy = y;
					}
					else
					{
						DrawLine(hdc, x, y, lx, ly);
					}

					lx = x;
					ly = y;

					j++;
					k++;
				}
				if (row->next)
				{
					last = row;
				}
				row = row->next;
			}
			if (i == our_i)
			{
				drawPoint(hwnd, x, y, 3, 0, 0, 0);
				if (k > 1)
					DrawLine(hdc, x, y, cx, cy);
				if (last)
				{
					if (k > 1)
						DrawLine(hdc, x, y, last->X, last->Y);
				}
			}
			else
			{
				if (k > 1)
					DrawLine(hdc, x, y, cx, cy);
			}
		}

	}
	ReleaseDC(hwnd, hdc);
}

void praint(HWND hWnd)
{
	int i = 0;

	RECT r;
	BITMAPINFO bitmap;
	GetClientRect(hWnd, &r);
	int MAX_WIDTH = r.right - r.left;
	int MAX_HEIGHT = r.bottom - r.top;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = MAX_WIDTH;
	bitmap.bmiHeader.biHeight = -MAX_HEIGHT;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 4 * MAX_HEIGHT;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;

	HDC hdc = GetDC(hWnd);
	HDC hdc2 = CreateCompatibleDC(hdc);
	BYTE* bitPointer;
	HBITMAP hBitmap2 = CreateDIBSection(NULL, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
	/*
	if (0 == GetDIBits(hdc2, hBitmap2, 0, 0, NULL, &bitmap, DIB_RGB_COLORS))
	{
	debug("error there", 1);
	// error handling
	}

	//

	BITMAP  BitMap;
	SelectObject(hdc, hBitmap2);
	GetObject(hBitmap2, sizeof(BITMAP), &BitMap);

	BITMAPINFOHEADER bInfo;

	bInfo.biSize = sizeof(bInfo); //эта штука 40 байт
	bInfo.biWidth = BitMap.bmWidth;
	bInfo.biHeight = BitMap.bmHeight;
	bInfo.biBitCount = 32;  //это количество бит на пиксель
	bInfo.biPlanes = 1;
	bInfo.biCompression = BI_RGB;


	BYTE * li = new BYTE[1024 * 768 * 4];
	int LineCopy = GetDIBits(hdc, hBitmap2, 0, BitMap.bmHeight, li, (LPBITMAPINFO)&bInfo, DIB_RGB_COLORS);
	int ch = 0;
	for (int x = 0; x < 1000; x++)
	{
	for (int y = 0; y < 200; y++)
	{
	ch += 3;
	if (RGB(li[ch], li[ch + 1], li[ch + 2]) == RGB(0, 0, 0))
	i++;
	li[ch] = 255;
	}
	}
	*/
	BitBlt(hdc, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc2, 0, 0, SRCCOPY);

	//
	/*
	for (int x = 0; x < 1000; x++)
	{
	for (int y = 0; y < 100; y++)
	{
	if (get_pixel_color(hBitmap2, x, y) == RGB(0, 0, 0))
	{
	i++;
	}
	}
	}
	*/
	/*
	int i = 0;
	for (int y = 0; y<MAX_HEIGHT; y++)
	for (int x = 0; x<MAX_WIDTH; x++)
	{
	bitPointer[y * MAX_WIDTH + x] == 0x00909000;
	//if (bitPointer[y * MAX_WIDTH + x] == 0x00000000)
	//{
	//	i++;
	//}//0x00787878;120 = 0x78
	;
	}

	BitBlt(hdc, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc2, 0, 0, SRCCOPY);
	DeleteObject(hBitmap2);
	ReleaseDC(NULL, hdc2);
	*/
	debug("black", i);

}

HBITMAP GetWindowBitmap(HWND hWnd)
{
	HWND hDesktop = ::GetDesktopWindow();
	HDC hDCDesktop = ::GetDC(hDesktop);

	HDC hDCWnd = ::GetWindowDC(hWnd);
	HDC hDCMem = ::CreateCompatibleDC(hDCWnd);

	RECT rectWnd;
	::GetWindowRect(hWnd, &rectWnd);
	HBITMAP hBmp = ::CreateCompatibleBitmap(hDCWnd, rectWnd.right - rectWnd.left, rectWnd.bottom - rectWnd.top);

	HGDIOBJ hTmp = ::SelectObject(hDCMem, hBmp);

	::BitBlt(hDCMem, 0, 0, rectWnd.right - rectWnd.left, rectWnd.bottom - rectWnd.top, hDCWnd, 0, 0, SRCCOPY);
	//      ::BitBlt(hDCMem, 0,0, rectWnd.right-rectWnd.left, rectWnd.bottom-rectWnd.top, hDCDesktop, rectWnd.left, rectWnd.top, SRCCOPY);
	//      ::PrintWindow(hWnd, hDCMem, 0);

	::SelectObject(hDCMem, hTmp);

	::DeleteObject(hDCMem);
	::ReleaseDC(hWnd, hDCWnd);
	::ReleaseDC(hDesktop, hDCDesktop);

	return hBmp;
}

void drawFigure(HWND hWnd)
{
	//drawLines(hWnd);
	Point point;

	HWND hwnd_x = GetDlgItem(hWnd, ID_ZEDIT_ZX);
	HWND hwnd_y = GetDlgItem(hWnd, ID_ZEDIT_ZY);

	TCHAR buffer1[BUFFER_MAX_SIZE];
	GetWindowText(hwnd_x, buffer1, sizeof(buffer1));

	TCHAR buffer2[BUFFER_MAX_SIZE];
	GetWindowText(hwnd_y, buffer2, sizeof(buffer2));

	point.x = atoi(buffer1);
	point.y = atoi(buffer2);

	debug("x", point.x);
	debug("y", point.y);

	RECT rect;
	HDC hdc = GetDC(hWnd);

	GetClientRect(hWnd, &rect);
	rect.left = point.x - 5;
	rect.right = point.x + 5;
	rect.top = point.y - 5;
	rect.bottom = point.y + 5;

	FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

	fill(hdc, point.x, point.y);

	ReleaseDC(hWnd, hdc);
}

// Отрисовка точек
void drawPoints(HWND hWnd, int clean)
{
	cleanRect(hWnd, 0, 0, 300, 0);

	Row *row = NULL;

	row = table->type;
	if (!row)
		return;
	HDC hdc = GetDC(hWnd);
	int cx = row->X, cy = row->Y, x = row->X, y = row->Y;
	int wx = x, wy = y;
	int max_i = table->max;
	int r = 255, g = 0, b = 0;
	int j = 0;
	int size;
	for (int i = 0; i <= max_i; i++)
	{
		j = 0;
		row = table->type;
		while (row != NULL)
		{
			if (row->i == i)
			{
				x = row->X;
				y = row->Y;

				if (!j)
				{
					cx = x;
					cy = y;
					wx = x;
					wy = y;
				}

				drawPoint(hWnd, x, y, Radius, r, g, b);
				DrawLine(hdc, x, y, cx, cy);
				cx = x;
				cy = y;
				j++;
			}
			row = row->next;
		}
		if (j)
		{
			DrawLine(hdc, wx, wy, cx, cy);
		}
	}
	ReleaseDC(hWnd, hdc);
}

void addListViewItem(HWND ListView, int row, int x, int y, int i)
{
	LVITEM lis;
	lis.lParam = 0;

	TCHAR buffer_x[BUFFER_MAX_SIZE];
	TCHAR buffer_y[BUFFER_MAX_SIZE];
	TCHAR buffer_t[BUFFER_MAX_SIZE];

	_snprintf_s(buffer_x, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", x);

	_snprintf_s(buffer_y, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", y);

	_snprintf_s(buffer_t, BUFFER_MAX_SIZE - 1, BUFFER_MAX_SIZE - 1, "%d", i);

	memset(&lis, 0, sizeof(LV_ITEM));

	lis.iItem = row;
	lis.iSubItem = 0;
	ListView_InsertItem(ListView, &lis);
	ListView_SetItemText(ListView, row, 0, buffer_x);

	lis.iSubItem = 1;
	ListView_InsertItem(ListView, &lis);
	ListView_SetItemText(ListView, row, 1, buffer_y);

	lis.iSubItem = 2;
	ListView_InsertItem(ListView, &lis);
	ListView_SetItemText(ListView, row, 2, buffer_t);
}

// Проверка на переполнение типа
int is_small(string s)
{
	if (s.length() > 8)
	{
		return 0;
	}
	return 1;
}

// Очистка памяти из под списка
Table* free_memory(Table *tab)
{
	Row *row = NULL, *tmp = NULL;

	row = tab->type;

	while (row != NULL)
	{
		tmp = row;
		row = row->next;
		free(tmp);
	}

	tab->type = NULL;

	return tab;
}

// Проверка, является ли строка числом http://www.cyberforum.ru/cpp-beginners/thread1858629.html
bool isStringNumber(string s)
{
	if (s.length() == 0)
	{
		return false;
	}
	for (int a = 0; a < s.length(); a++)
	{
		// Если в строке есть не цифровые символы, то это не число
		if (s[a] != '-')
		{ // Проверка на знак минус
			if (s[a] != '.') // Проверка на точку
			{
				if ((s[a] < 48) || (s[a] > 57))
				{
					return false;
				}
			}
		}
	} // Если в строке только цифровые символы значит это число
	return true;
}

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

// Получить количество элементов в списке
int getTableSize(Table *table)
{
	int size = 0;
	Row *row = NULL;
	row = table->type;
	while (row != NULL)
	{
		size++;
		row = row->next;
	}
	return size;
}

// Рисование точки
void drawPoint(HWND hWnd, double XBuf, double YBuf, int radius, int r, int g, int b)
{
	HDC hdc = GetDC(hWnd);
	HBRUSH hBrush = CreateSolidBrush(RGB(r, g, b));

	SelectObject(hdc, hBrush);
	Ellipse(hdc, XBuf - radius, YBuf - radius, XBuf + radius, YBuf + radius);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);
}

// Рисование линии для сетки
void DrawLine(HDC hdc, int X1, int Y1, int X2, int Y2)
{
	MoveToEx(hdc, X1, Y1, NULL);
	LineTo(hdc, X2, Y2);
}

int roun(const float a)
{
	return int(a + 0.5);
}

void sortStrok(Stroka* begin)
{
	Stroka *prev = begin, *curr = prev->next;

	if ((!prev) || (!curr))
	{
		return;
	}

	int tmp;

	if (curr->x < prev->x)
	{
		tmp = curr->x;
		curr->x = prev->x;
		prev->x = tmp;
	}

	while (curr != NULL)
	{
		if (curr->x < prev->x)
		{
			tmp = curr->x;
			curr->x = prev->x;
			prev->x = tmp;

			prev = begin;
			curr = prev->next;
		}
		else
		{
			curr = curr->next;
			prev = prev->next;
		}
	}
}

bool finderStrok(HWND hWnd, Stroka *begin, int x)
{
	if (!begin)
		return FALSE;
	Stroka *curr = begin->next;
	Stroka *prev = begin;

	bool tr = FALSE;

	while (curr != NULL)
	{
		if (nea(prev->x, x))
		{
			prev->y = -1;
			tr = TRUE;
			//break;
		}
		if (nea(curr->x, x))
		{
			curr->y = -1;
			tr = TRUE;
			//break;
		}
		prev = prev->next;
		curr = curr->next;
	}
	return tr;
}

void peresech(HWND hWnd, Stroka **original, Rebro *figure, int y, int i)
{
	Rebro *move = figure;
	Stroka *str = *original;
	Stroka *prev = str;

	while (str != NULL)
	{
		prev = str;
		str = str->next;
	}

	int size = 0;
	int xstr = 0;
	while ((move != NULL))
	{
		if ((y <= move->y) && (y >= move->y - move->dy))
		{
			xstr = move->x + (move->y - y) * move->dx + 1;


			str = makeStr((move->x + (move->y - y) * move->dx + 1), y, i);
			if (!size)
			{
				if (!(*original))
				{
					*original = str;
					prev = str;
				}
				else
				{
					prev->next = str;
					prev = prev->next;
				}
			}
			else
			{
				prev->next = str;
				prev = prev->next;
			}
			size++;
			str = str->next;
		}
		move = move->next;
	}

	sortStrok(*original);
}
// Вставки

Point *allocatePoint(int x, int y)
{
	Point* point = (Point*)calloc(1, sizeof(Point));
	point->x = x;
	point->y = y;
	return point;
}

Stack* allocateElement(Point *point)
{
	Stack* el = (Stack*)calloc(1, sizeof(Stack));
	el->point = point;
	el->next = NULL;
	return el;
}

void paintPixel(HDC hdc, int Xc, int Yc, int dx, int dy, DWORD cur)
{
	SetPixel(hdc, Xc + dx, Yc + dy, cur);
	SetPixel(hdc, Xc - dx, Yc + dy, cur);
	SetPixel(hdc, Xc + dx, Yc - dy, cur);
	SetPixel(hdc, Xc - dx, Yc - dy, cur);
}

void drawEllipseBr(HDC hdc, int cx, int cy, int rx, int ry, DWORD cur)
{
	int rx2 = rx * rx;//a^2
	int ry2 = ry * ry;//b^2
	int r2y2 = 2 * ry2;
	int r2x2 = 2 * rx2;
	int x = 0, y = ry;

	int d = rx2 + ry2 - r2x2 * y;
	int d1, d2;

	while (y >= 0)
	{
		paintPixel(hdc, cx, cy, x, y, cur);

		if (d < 0)//гор и диаг
		{
			d1 = 2 * d + r2x2 * y - 1;
			if (d1 > 0) //диагональная
			{
				y -= 1;
				x += 1;
				d += r2y2 * x + ry2 + rx2 - r2x2 * y;//b^2 (2x+1)+a^2(1-2y)
			}
			else     //гор
			{
				x += 1;
				d += r2y2 * x + ry2;    //+b^2 (2x+1)
			}
		}
		else if (d == 0)//диагональная
		{
			x += 1;
			y -= 1;
			d += r2y2 * x + ry2 + rx2 - r2x2 * y;
		}
		else
		{
			d2 = 2 * d - r2y2 * x - 1; //2d-b^2x-1
			if (d2 < 0) //диагональная
			{
				y -= 1;
				x += 1;
				d += r2y2 * x + ry2 + rx2 - r2x2 * y;
			}
			else//вертикальная
			{
				y -= 1;
				d += rx2 - r2x2 * y;  //a^2(1-2y)
			}
		}
	} //end while
}

void push(Stack **stack, Point *point)
{
	if (!(*stack))
	{
		*stack = allocateElement(point);
		return;
	}

	Stack *mover = *stack;
	while (mover->next)
	{
		mover = mover->next;
	}
	Stack *insert = allocateElement(point);
	mover->next = insert;
}

Point* pop(Stack **stack)
{
	if (!(*stack))
		return NULL;

	Point* point = (*stack)->point;

	*stack = (*stack)->next;

	return  point;
}

DWORD **allocateMatrix(int n, int m)
{
	DWORD **data = (DWORD**)calloc(1, n * sizeof(DWORD*) + n * m * sizeof(DWORD));
	if (!data)
	{
		return NULL;
	}

	for (int i = 0; i < n; i++)
	{
		data[i] = (DWORD*)((char*)data + n * sizeof(DWORD*) +
			i * m * sizeof(DWORD));
	}
	return data;
}

void toMatrix(int columns, int number, int *i, int *j)
{
	*i = number / columns;
	*j = number % columns;
}

bool isBorder(HDC hdc, int x, int y, DWORD color)
{
	return (
		(GetPixel(hdc, x, y) == color) ||
		(x <= 0) || (x >= 600)
		);
}

int goToBorder(HDC hdc, Point *point, int where)
{
	DWORD black = RGB(0, 0, 0);
	int x = point->x;
	int y = point->y;
	if (isBorder(hdc, x, y, black))
		return x;
	int i;
	if (where == LEFT)
		i = -1;
	else
		i = 1;
	do
	{
		SetPixel(hdc, x, y, black);
		x += i;
	} while (!isBorder(hdc, x, y, black));

	return x;
}

void searchZ(HDC hdc, Stack **stack,
	int xl, int xr, int Y)
{
	int X = xl;
	bool flag = FALSE;
	DWORD black = RGB(0, 0, 0);
	Point *point;
	//
	do
	{
		flag = FALSE;
		//ищем грань идя направо
		while ((X <= xr) &&
			(!isBorder(hdc, X, Y, black)))
		{
			X++;
			if (isBorder(hdc, X, Y, black))
			{
				flag = TRUE;
			}
		}

		if (flag)
		{
			if ((X == xr) &&
				(!isBorder(hdc, X, Y, black)))
			{
				point = allocatePoint(X, Y);
				push(stack, point);
				debug("push", Y);
				//break;
			}
			else
			{//Если оказались на гране то смещаемся на один левее
				point = allocatePoint(X - 1, Y);
				push(stack, point);
				debug("push", Y);
				//break;
			}
		}

		X++;
	} while (X - 1<xr);
	//
	/*
	do
	{
	flag = FALSE;
	//ищем грань идя направо
	while ((X <= xr) &&
	(!isBorder(hdc, max, X, Y, black)))
	{
	X++;
	flag = TRUE;
	}

	if (flag)
	{
	if ((X == xr) &&
	(!isBorder(hdc, max, X, Y, black)))
	{
	point = allocatePoint(X, Y);
	push(stack, point);
	debug("push", Y);
	//break;
	}
	else
	{//Если оказались на гране то смещаемся на один левее
	point = allocatePoint(X - 1, Y);
	push(stack, point);
	debug("push", Y);
	//break;
	}
	}

	X++;
	} while (X - 1<xr);
	*/
}

void fill(HDC hdc, int zx, int zy)
{
	Point start;
	start.x = zx;
	start.y = zy;
	Stack *stack = allocateElement(&start);
	Point *point;
	DWORD black = RGB(0, 0, 0);
	int x, xl, xr, y, i, xt, fl;


	while (stack)
	{
		point = pop(&stack);

		x = point->x;
		y = point->y;

		if ((y > 800) || (y < 1))
			continue;

		xt = x;
		fl = 0;
		SetPixel(hdc, x, y, black);
		x--;
		while (!isBorder(hdc, x, y, black))
		{
			SetPixel(hdc, x, y, black);
			x--;
		}

		xl = x + 1;
		x = xt;
		x++;
		while (!isBorder(hdc, x, y, black))
		{
			SetPixel(hdc, x, y, black);
			x++;
		}

		xr = x - 1;
		y++;
		x = xl;
		while (x < xr)
		{
			fl = 0;
			while ((!isBorder(hdc, x, y, black)) && (x < xr))
			{
				if (!fl)
					fl = 1;
				x++;
			}
			if (fl)
			{
				if ((x == xr) && (!isBorder(hdc, x, y, black)))
				{
					Point* p = allocatePoint(x, y);
					push(&stack, p);
				}
				else
				{
					Point* p = allocatePoint(x - 1, y);
					push(&stack, p);
				}
				fl = 0;
			}
			xt = x;
			while ((isBorder(hdc, x, y, black)) && (x < xr))
			{
				x++;
			}

			if (x == xt)
			{
				x++;
			}
		}

		y = y - 2;
		x = xl;

		while (x < xr)
		{
			fl = 0;
			while ((!isBorder(hdc, x, y, black)) && (x < xr))
			{
				if (!fl)
					fl = 1;
				x++;
			}
			if (fl)
			{
				if ((x == xr) && (!isBorder(hdc, x, y, black)))
				{
					Point* p = allocatePoint(x, y);
					push(&stack, p);
				}
				else
				{
					Point* p = allocatePoint(x - 1, y);
					push(&stack, p);
				}
				fl = 0;
			}
			xt = x;
			while ((isBorder(hdc, x, y, black)) && (x < xr))
			{
				x++;
			}

			if (x == xt)
			{
				x++;
			}
		}
	}
}

void fiill(HDC hdc, int zx, int zy)
{
	Point start;
	start.x = zx;
	start.y = zy;
	Stack *stack = allocateElement(&start);
	Point *point;
	DWORD black = RGB(0, 0, 0);
	int x, xl, xr, y, i;


	while (stack)
	{
		point = pop(&stack);
		x = point->x;
		y = point->y;

		if ((y > 800) || (y < 1))
			continue;

		xr = goToBorder(hdc, point, RIGHT);
		point->x = point->x - 1;
		xl = goToBorder(hdc, point, LEFT);

		searchZ(hdc, &stack, xl, xr, y - 1);
		//searchZ(hdc, &stack, max, xr, xr, y - 1);
	}
	start.x = zx;
	start.y = zy + 1;
	Stack *stack1 = allocateElement(&start);
	while (stack1)
	{
		debug("pop1", 0);
		point = pop(&stack1);
		x = point->x;
		y = point->y;

		xr = goToBorder(hdc, point, RIGHT);
		xl = goToBorder(hdc, point, LEFT);

		searchZ(hdc, &stack1, xl, xr, y + 1);
		//searchZ(hdc, &stack, max, xr, xr, y - 1);
	}
}

///////////

void drawCutInside(HWND hWnd, Cut cut,
	Secatel sec)
{
	Point* first = newPoint(NO_POINT, NO_POINT);
	Point* second = newPoint(NO_POINT, NO_POINT);
	Cut *ncut = cutInside(cut, sec);

	Point first = ncut->begin;
	Point second = ncut->end;
	HDC hdc = GetDC(hWnd);
	DrawLine(hdc, first->x, first->y,
		second->x, second->y);
	ReleaseDC(hWnd, hdc);
}