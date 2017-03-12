#pragma once

#include <Windows.h>

typedef struct {
	INT count;
	HWND paintWin;
	HWND massSlider;
	HWND lengthSlider;
} MainWinData;

#define MAINWINCLASS L"MainWindowClassName"

ATOM RegisterMainWin(HINSTANCE hInstance);
LRESULT CALLBACK MainWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);