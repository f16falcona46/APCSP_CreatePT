#pragma once

/* internal data for window */
typedef struct {
	POINT pendulumPos;
	POINT pendulumVelocity;
	DOUBLE pendulumMass;
	INT count;
	DOUBLE phase;
	DOUBLE pendulumLength;
} PendulumData;

/* offset for GetWindowLongPtr */
#define PENDULUMDATA_OFFSET 0

/* PaintWinProc
	a normal window proc. Don't touch this!
*/
LRESULT CALLBACK PaintWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* draws pendulum based on internal data */
void DrawPendulum(HWND hwnd, HDC hdc, PAINTSTRUCT* ps, const PendulumData* data);

/* updates pendulum; interval is number of ms elapsed since last tick */
void UpdatePendulum(HWND hwnd, PendulumData* data, INT interval);