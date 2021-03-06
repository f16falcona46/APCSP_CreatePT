#include "PaintWin.h"
#include "PaintWinPriv.h"
#include "IDs.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>

ATOM RegisterPaintWin(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = PaintWinProc;
	wc.hInstance = hInstance;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(void*);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszClassName = PAINTWINCLASS;

	return RegisterClassEx(&wc);
}

LRESULT CALLBACK PaintWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
	{
		PendulumData* data;
		data = (PendulumData*)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(PendulumData));
		if (!data) {
			MessageBox(hwnd, L"Unable to allocate memory.", L"Error", MB_ICONERROR | MB_OK);
			return -1;
		}
		InitPaintWinData(data);

		/* TODO remove this placeholder */
		//data->pendulumLength = 100;
		//data->pendulumMass = 30;
		/* end TODO */

		SetWindowLongPtr(hwnd, PENDULUMDATA_OFFSET, (LONG_PTR)data);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		const PendulumData* data = (PendulumData*)GetWindowLongPtr(hwnd, PENDULUMDATA_OFFSET);
		hdc = BeginPaint(hwnd, &ps);
		DrawPendulum(hwnd, hdc, &ps, data);
		EndPaint(hwnd, &ps);
	}
	break;
	case PWM_UPDATE_TICK:
	{
		PendulumData* data = (PendulumData*)GetWindowLongPtr(hwnd, PENDULUMDATA_OFFSET);
		assert(lParam == (LPARAM)NULL);
		UpdatePendulum(hwnd, data, (INT)wParam);
	}
	break;
	case PWM_SETPENDULUMLENGTH:
	{
		PendulumData* data = (PendulumData*)GetWindowLongPtr(hwnd, PENDULUMDATA_OFFSET);
		assert(wParam == 0);
		data->pendulumLength = *(DOUBLE*)lParam;
	}
	break;
	case PWM_SETPENDULUMMASS:
	{
		PendulumData* data = (PendulumData*)GetWindowLongPtr(hwnd, PENDULUMDATA_OFFSET);
		assert(wParam == 0);
		data->pendulumMass = *(DOUBLE*)lParam;
	}
	break;
	case PWM_GETPENDULUMLENGTH:
	{
		const PendulumData* data = (PendulumData*)GetWindowLongPtr(hwnd, PENDULUMDATA_OFFSET);
		assert(wParam == 0);
		*(DOUBLE*)lParam = data->pendulumLength;
	}
	break;
	case PWM_GETPENDULUMMASS:
	{
		const PendulumData* data = (PendulumData*)GetWindowLongPtr(hwnd, PENDULUMDATA_OFFSET);
		assert(wParam == 0);
		*(DOUBLE*)lParam = data->pendulumMass;
	}
	break;
	case WM_SIZE:
	{
		PendulumData* data = (PendulumData*)GetWindowLongPtr(hwnd, PENDULUMDATA_OFFSET);
		UpdatePendulum(hwnd, data, 0);
	}
	break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

void DrawPendulum(HWND hwnd, HDC hdc, PAINTSTRUCT* ps, const PendulumData* data)
{
	RECT rc;

	GetClientRect(hwnd, &rc);

	MoveToEx(hdc, rc.right / 2, rc.top, NULL);
	LineTo(hdc, data->pendulumPos.x, data->pendulumPos.y);

	Ellipse(hdc, (INT)(data->pendulumPos.x - data->pendulumMass / 2),
		(INT)(data->pendulumPos.y - data->pendulumMass / 2),
		(INT)(data->pendulumPos.x + data->pendulumMass / 2),
		(INT)(data->pendulumPos.y + data->pendulumMass / 2));
}

void UpdatePendulum(HWND hwnd, PendulumData* data, INT interval)
{
	RECT rc;
	data->phase += interval / sqrt(data->pendulumLength) / 60.0;
	if (data->phase > 2 * M_PI) data->phase -= 2 * M_PI;
	GetClientRect(hwnd, &rc);
	data->pendulumPos.x = (LONG)(data->pendulumLength * sin(sin(data->phase) / 4) + rc.right / 2);
	data->pendulumPos.y = (LONG)(data->pendulumLength * cos(sin(data->phase) / 4));
	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
}

void InitPaintWinData(PendulumData* data)
{
	data->pendulumPos.x = 0;
	data->pendulumPos.y = 0;
	data->pendulumVelocity.x = 0;
	data->pendulumVelocity.y = 0;
	data->pendulumMass = 1.0;
	data->phase = 0.0;
	data->pendulumLength = 1.0;
}