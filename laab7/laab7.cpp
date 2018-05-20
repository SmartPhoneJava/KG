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



#include "Point.h"
#include "Cut.h"
#include "Debug.h"
#include "Alg.h"
#include "Secatel.h"
#include "Graphics.h"
#include "Table.h"
#include "Resource.h"
#include "Interface.h"

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

int listview_choosen = 0;
int listview_x1 = 0;
int listview_y1 = 0;
int listview_x2 = 0;
int listview_y2 = 0;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void greatRewrite(HWND hWnd, int with_clean);

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

	static bool lclicked = FALSE;
	static bool rclicked = FALSE;
	static bool ctrl = FALSE;
	static bool shift = FALSE;

	static Table* table = NULL;

	static int lx = NO_POINT, ly = NO_POINT;
	static int rx = NO_POINT, ry = NO_POINT;
	static int count = 0;

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
		CreateWindow(text_button, text_clear, WS_CHILD | BS_PUSHBUTTON,
			rc.right - 200, 22, 100, 20, hWnd, (HMENU)ID_BUTTON_CLEAR, NULL, NULL);
		CreateWindow(text_button, text_change, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			rc.right - 100, 2, 98, 20, hWnd, (HMENU)ID_BUTTON_CHANGE, NULL, NULL);
		CreateWindow(text_edit, text_0, WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.right - 280, 50, 170, 20, hWnd, (HMENU)ID_EDIT_X1, NULL, NULL);
		CreateWindow(text_edit, text_x, WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.right - 300, 50, 15, 20, hWnd, (HMENU)ID_NEDIT_X1, NULL, NULL);
		CreateWindow(text_edit, text_0, WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.right - 280, 75, 170, 20, hWnd, (HMENU)ID_EDIT_Y1, NULL, NULL);
		CreateWindow(text_edit, text_y, WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.right - 300, 75, 15, 20, hWnd, (HMENU)ID_NEDIT_Y1, NULL, NULL);

		CreateWindow(WC_LISTVIEW, text_1, WS_CHILD | LVS_REPORT | LVS_EX_FULLROWSELECT | WS_VSCROLL | WS_VISIBLE,
			rc.right - 300, 125, 300, rc.bottom - 100, hWnd, (HMENU)ID_LISTVIEW, NULL, NULL);

		EnableWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), FALSE);
		EnableWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), FALSE);
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

		SendDlgItemMessage(hWnd, ID_NEDIT_X1, EM_SETREADONLY, 1, 0);
		SendDlgItemMessage(hWnd, ID_NEDIT_Y1, EM_SETREADONLY, 1, 0);

		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		rclicked = TRUE;
		mouseClicked(lParam, 700, count, rx, ry, rclicked);
		break;
	}
	case WM_RBUTTONUP:
	{
		rclicked = FALSE;
		break;
	}
	case WM_LBUTTONUP:
	{
		lclicked = FALSE;
		break;
	}
	case WM_LBUTTONDOWN:
	{
		lclicked = TRUE;
		mouseClicked(lParam, 700, count, lx, ly, lclicked);
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (lclicked)
			mouseClicked(lParam, 700, count, lx, ly, lclicked);
		else if (rclicked)
			mouseClicked(lParam, 700, count, rx, ry, rclicked);
		else
			break;

		debug("here", 0);

		Cut *cut = newCutInt(lx, ly, rx, ry);
		debugCut(cut, "cutt", 0);
		table = addToTable(table, cut);
		debugTable(table, "debugTable", 7);
		HDC hdc = GetDC(hWnd);
		drawTable(hdc, table, RGB(0, 0, 255));
		ReleaseDC(hWnd, hdc);
		table = deleteOfTable(table, cut);
		deleteCut(&cut);

		debug("finish", 0);
		break;
	}
	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}

	//https://stackoverflow.com/questions/18745447/how-can-i-change-the-background-color-of-a-button-winapi-c
	
	// перемещение окна
	case WM_SIZE:
	{
		greatRewrite(hWnd, 0);

		break;
	}
	// http://www.cyberforum.ru/win-api/thread1098433.html
	case WM_NOTIFY:
	{
		HWND edit_x = GetDlgItem(hWnd, ID_EDIT_X1);
		HWND edit_y = GetDlgItem(hWnd, ID_EDIT_Y1);
		HWND nedit_x = GetDlgItem(hWnd, ID_NEDIT_X1);
		HWND nedit_y = GetDlgItem(hWnd, ID_NEDIT_Y1);
		HWND listview = GetDlgItem(hWnd, ID_LISTVIEW);

		switch (((LPNMHDR)lParam)->code)
		{
		case LVN_ITEMCHANGED:
		{
			updatingListView(hWnd, lParam,
				listview_choosen, listview_x1,
				listview_y1, listview_x2,
				listview_y2);
		}
		}
	}
	case WM_COMMAND:
	{

		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case ID_BUTTON_BUILD:
		{
			Point *p1 = newPoint(50, 50);
			Point *p2 = newPoint(400, 200);

			Secatel *sec = newSecatel(100,
				300,180,100);

			setPointVisibility(*p1, *sec);
			setPointVisibility(*p2, *sec);

			Cut *cut = newCut(p1, p2);

			//debugVisibility(cut->begin.vis, "1-st point visibility", isPointVisible(p1->vis));
			//debugVisibility(cut->end.vis, "2-st point visibility", isPointVisible(p2->vis));

			HDC hdc = GetDC(hWnd);

			drawCut(hdc, cut, BLUE);
			drawSecatel(hdc, *sec, GREEN);
			drawCutInside(hdc, cut, *sec, RED);
			//drawFigure(hWnd);
			ReleaseDC(hWnd, hdc);
			break;
		}
		case ID_BUTTON_DELETE:
		{
			if (getTableSize(table) == 0)
			{
				MessageBox(NULL, "There is no any point. You cant delete anything!",
					"Warning", MB_OK);
			}
			else
			{
				deleting(hWnd, listview_choosen,
					listview_x1, listview_y1,
					listview_x2, listview_y2, table);
			}
			break;
		}
		case ID_BUTTON_CHANGE:
		{
			if (getTableSize(table) == 0)
			{
				MessageBox(NULL, "There is no any point. You cant change anything!",
					"Warning", MB_OK);
			}
			else
			{
				changing(hWnd, listview_choosen, table,
					listview_x1, listview_y1,
					listview_x2, listview_y2);
			}
			
			break;
		}
		/*
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
		*/
		case ID_BUTTON_ADD:
		{
			table = adding(hWnd, table);
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
		cleanRectOld(hWnd, 0, 0, 300, 0);
	}

	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_ADD), rc.right - 300, 2, 100, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_DELETE), rc.right - 200, 2, 100, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_BUILD), rc.right - 300, 22, 100, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_CLEAR), rc.right - 200, 22, 100, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_BUTTON_CHANGE), rc.right - 100, 2, 98, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_NEDIT_X1), rc.right - 300, 50, 15, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_EDIT_X1), rc.right - 280, 50, 170, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_EDIT_Y1), rc.right - 280, 75, 170, 20, 0);
	MoveWindow(GetDlgItem(hWnd, ID_NEDIT_Y1), rc.right - 300, 75, 15, 20, 0);

	MoveWindow(GetDlgItem(hWnd, ID_LISTVIEW), rc.right - 300, 140, 300, rc.bottom - 100, 0);

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
