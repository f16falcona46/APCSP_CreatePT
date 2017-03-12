#include "PaintWin.h"
#include "PaintWinPriv.h"
#include "IDs.h"

#include <math.h>

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
		INT rc;
		data = (PendulumData*)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(PendulumData));
		if (!data) {
			MessageBox(hwnd, L"Unable to allocate memory.", L"Error", MB_ICONERROR | MB_OK);
			return -1;
		}
		SetWindowLongPtr(hwnd, PENDULUMDATA_OFFSET, data);
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
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_PAINTWINUPDATE:
		{
			PendulumData* data = (PendulumData*)GetWindowLongPtr(hwnd, PENDULUMDATA_OFFSET);
			UpdatePendulum(hwnd, data, lParam);
		}
		break;
		}
		break;
	case WM_SIZE:
	{
		PendulumData* data = (PendulumData*)GetWindowLongPtr(hwnd, PENDULUMDATA_OFFSET);
		UpdatePendulum(hwnd, data, 0);
	}
	break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

void DrawPendulum(HWND hwnd, HDC hdc, PAINTSTRUCT* ps, const PendulumData* data)
{
	RECT rc;
	WCHAR buf[50];

	wsprintf(buf, L"%d", data->count);

	GetClientRect(hwnd, &rc);
	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, buf, -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	MoveToEx(hdc, rc.right / 2, rc.top, NULL);
	LineTo(hdc, data->pendulumPos.x, data->pendulumPos.y);
}

void UpdatePendulum(HWND hwnd, PendulumData* data, INT interval)
{
	RECT rc;
	data->count += interval;
	GetClientRect(hwnd, &rc);
	data->pendulumPos.x = rc.right / 2 * (sin(data->count / 500.0) + 1);
	data->pendulumPos.y = rc.bottom;
	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
}