#pragma once

#include <windows.h>

#define MAINWINCLASS L"MainWindowClassName"

ATOM RegisterMainWin(HINSTANCE hInstance);
LRESULT CALLBACK MainWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);