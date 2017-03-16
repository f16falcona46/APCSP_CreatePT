#pragma once

#include "MainWin.h"

/* internal data for mainwin */
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

/* offset for mainwin internal data ptr, for GetWindowLongPtr */
#define MAINWINDATA_OFFSET 0

/* timer interval in milliseconds */
#define MAINWINTIMER_INTERVAL (1000/60)

/* window proc for mainwin. don't touch this! */
LRESULT CALLBACK MainWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);