#pragma once

#include <Windows.h>

typedef struct {
	INT count;
	HWND paintWin;
	HWND massSlider;
	HWND massSliderNameLabel;
	HWND massSliderMinLabel;
	HWND massSliderMaxLabel;
	HWND massSliderValueLabel;
	HWND lengthSlider;
	HWND lengthSliderNameLabel;
	HWND lengthSliderMinLabel;
	HWND lengthSliderMaxLabel;
	HWND lengthSliderValueLabel;
} MainWinData;

#define MAINWINCLASS L"MainWindowClassName"

ATOM RegisterMainWin(HINSTANCE hInstance);
LRESULT CALLBACK MainWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);