#pragma once

#include <windows.h>

typedef struct {
	INT count;
	HWND paintWin;
} MainWinData;

#define MAINWINCLASS L"MainWindowClassName"

ATOM RegisterMainWin(HINSTANCE hInstance);
LRESULT CALLBACK MainWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);