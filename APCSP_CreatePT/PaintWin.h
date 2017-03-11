#pragma once

#include <Windows.h>

typedef struct {
	POINT pendulumPos;
	POINT pendulumVelocity;
	DOUBLE pendulumMass;
	INT count;
} PendulumData;

#define PAINTWINCLASS L"PaintWindowClassName"

ATOM RegisterPaintWin(HINSTANCE hInstance);
LRESULT CALLBACK PaintWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);