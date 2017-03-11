#pragma once

#include <windows.h>

typedef struct {
	POINT pendulumPos;
	POINT pendulumVelocity;
	DOUBLE pendulumMass;
	INT count;
	HWND paintWin;
} MainWinData;

#define MAINWINCLASS L"MainWindowClassName"

ATOM RegisterMainWin(HINSTANCE hInstance);
LRESULT CALLBACK MainWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);