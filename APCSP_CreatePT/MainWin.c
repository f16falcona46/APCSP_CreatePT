#include "MainWin.h"
#include "MainWinPriv.h"
#include "IDs.h"
#include "PaintWin.h"

#include <math.h>

ATOM RegisterMainWin(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = MainWinProc;
	wc.hInstance = hInstance;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(void*);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = MAINWINCLASS;

	return RegisterClassEx(&wc);
}

LRESULT CALLBACK MainWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
	{
		const CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
		MainWinData* data;
		RECT rc;
		data = (MainWinData*)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(MainWinData));
		if (!data) {
			MessageBox(hwnd, L"Unable to allocate memory.", L"Error", MB_ICONERROR | MB_OK);
			return -1;
		}
		SetWindowLongPtr(hwnd, MAINWINDATA_OFFSET, data);
		if (!SetTimer(hwnd, ID_UPDATETIMER, MAINWINTIMER_INTERVAL, NULL)) {
			MessageBox(hwnd, L"Unable to set timer.", L"Error", MB_ICONERROR | MB_OK);
			return -1;
		}
		if (!RegisterPaintWin(cs->hInstance)) {
			MessageBox(hwnd, L"Unable to register paintwin.", L"Error", MB_ICONERROR | MB_OK);
			return -1;
		}
		GetClientRect(hwnd, &rc);
		HWND paintWin = CreateWindow(PAINTWINCLASS, "Paint window", WS_CHILD | WS_VISIBLE, 0, 0, rc.right, rc.bottom, hwnd, NULL, cs->hInstance, NULL);
		if (!paintWin) {
			MessageBox(hwnd, L"Unable to create paintwin.", L"Error", MB_ICONERROR | MB_OK);
			return -1;
		}
		data->paintWin = paintWin;
	}
	break;
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 300;
		lpMMI->ptMinTrackSize.y = 200;
	}
	break;
	case WM_SIZE:
	{
		const MainWinData* data = (MainWinData*)GetWindowLongPtr(hwnd, MAINWINDATA_OFFSET);
		RECT rc;
		GetClientRect(hwnd, &rc);
		MoveWindow(data->paintWin, 0, 0, rc.right - 50, rc.bottom, TRUE);
	}
	break;
	case WM_TIMER:
		switch (wParam) {
		case ID_UPDATETIMER:
		{
			MainWinData* data = (MainWinData*)GetWindowLongPtr(hwnd, MAINWINDATA_OFFSET);
			data->count += MAINWINTIMER_INTERVAL;
			SendMessage(data->paintWin, WM_COMMAND, ID_PAINTWINUPDATE, MAINWINTIMER_INTERVAL);
		}
		break;
		default:
			MessageBox(hwnd, L"Unexpected timer.", L"Error", MB_ICONERROR | MB_OK);
			return -1;
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