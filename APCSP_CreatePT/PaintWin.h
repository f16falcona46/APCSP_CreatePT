#pragma once

#include <Windows.h>

/* PAINTWINCLASS
	use this to instantiate the window
*/
#define PAINTWINCLASS L"PaintWindowClassName"


/* Window messages */

/* PWM_UPDATE_TICK
	wParam: number of elapsed milliseconds since last tick
	lParam: must be NULL
*/
#define PWM_UPDATE_TICK (WM_APP)

/* PWM_SETPENDULUMLENGTH
	wParam: must be 0
	lParam: pointer to a DOUBLE (in m)
*/
#define PWM_SETPENDULUMLENGTH (WM_APP+1)

/* PWM_SETPENDULUMMASS
	wParam: must be 0
	lParam: pointer to a DOUBLE (in kg)
*/
#define PWM_SETPENDULUMMASS (WM_APP+2)

/* PWM_GETPENDULUMLENGTH
	wParam: must be 0
	lParam: pointer to a DOUBLE (in m)
*/
#define PWM_GETPENDULUMLENGTH (WM_APP+3)

/* PWM_GETPENDULUMMASS
	wParam: must be 0
	lParam: pointer to a DOUBLE (in kg)
*/
#define PWM_GETPENDULUMMASS (WM_APP+4)


/* functions */

/* RegisterPaintWin
	returns NULL for failure, just like RegisterClassEx.
	otherwise returns the ATOM from it
*/
ATOM RegisterPaintWin(HINSTANCE hInstance);