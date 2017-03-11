#include "MainWin.h"
#include "MainWinPriv.h"
#include "IDs.h"

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
		MainWinData* data;
		INT rc;
		data = (MainWinData*)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(MainWinData));
		if (!data) {
			MessageBox(hwnd, L"Unable to allocate memory.", "Error", MB_ICONERROR | MB_OK);
			return -1;
		}
		SetWindowLongPtr(hwnd, MAINWINDATA_OFFSET, data);
		rc = SetTimer(hwnd, IDI_UPDATETIMER, 500, NULL);
		if (!rc) {
			MessageBox(hwnd, L"Unable to set timer.", "Error", MB_ICONERROR | MB_OK);
			return -1;
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		RECT rc;
		MainWinData* data;
		WCHAR buf[50];

		data = (MainWinData*)GetWindowLongPtr(hwnd, MAINWINDATA_OFFSET);
		wsprintf(buf, L"%d", data->count);
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);
		SetTextColor(hdc, RGB(0, 0, 0));
		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, buf, -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 300;
		lpMMI->ptMinTrackSize.y = 200;
	}
	break;
	case WM_TIMER:
		switch (wParam) {
		case IDI_UPDATETIMER:
		{
			MainWinData* data;
			data = (MainWinData*)GetWindowLongPtr(hwnd, MAINWINDATA_OFFSET);
			++data->count;
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
		}
		break;
		default:
			MessageBox(hwnd, "Unexpected timer.", "Error", MB_ICONERROR | MB_OK);
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