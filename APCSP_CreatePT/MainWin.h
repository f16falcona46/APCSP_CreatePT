#pragma once

#include <Windows.h>

/* MAINWINCLASS
	use this to instantiate the window
*/
#define MAINWINCLASS L"MainWindowClassName"

/* RegisterMainWin
	returns NULL for failure, just like RegisterClassEx
	otherwise returns the ATOM from it
*/
ATOM RegisterMainWin(HINSTANCE hInstance);