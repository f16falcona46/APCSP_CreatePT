#include "MainWin.h"
#include "MainWinPriv.h"
#include "IDs.h"
#include "PaintWin.h"
#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
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
		INITCOMMONCONTROLSEX initctrls;
		HFONT dialogFont;

		data = (MainWinData*)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(MainWinData));
		if (!data) {
			MessageBox(hwnd, L"Unable to allocate memory.", L"Error", MB_ICONERROR | MB_OK);
			return -1;
		}
		SetWindowLongPtr(hwnd, MAINWINDATA_OFFSET, (LONG_PTR)data);
		if (!SetTimer(hwnd, ID_UPDATETIMER, MAINWINTIMER_INTERVAL, NULL)) {
			MessageBox(hwnd, L"Unable to set timer.", L"Error", MB_ICONERROR | MB_OK);
			return -1;
		}
		if (!RegisterPaintWin(cs->hInstance)) {
			MessageBox(hwnd, L"Unable to register paintwin.", L"Error", MB_ICONERROR | MB_OK);
			return -1;
		}
		GetClientRect(hwnd, &rc);
		data->paintWin = CreateWindow(PAINTWINCLASS, L"Paint window", WS_CHILD | WS_VISIBLE, 0, 0, rc.right, rc.bottom, hwnd, NULL, cs->hInstance, NULL);

		initctrls.dwSize = sizeof(initctrls);
		initctrls.dwICC = ICC_BAR_CLASSES;
		if (!InitCommonControlsEx(&initctrls)) {
			MessageBox(hwnd, L"Unable to init common controls.", L"Error", MB_ICONERROR | MB_OK);
			return -1;
		}
		data->massSlider = CreateWindowEx(0, TRACKBAR_CLASS, L"Mass", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 0, 0, 10, 10, hwnd, NULL, cs->hInstance, NULL);
		data->massSliderNameLabel = CreateWindow(L"STATIC", L"MSliderNameLabel", WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 0, 10, 10, hwnd, NULL, cs->hInstance, NULL);
		data->massSliderMinLabel = CreateWindow(L"STATIC", L"MSliderMinLabel", WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 0, 10, 10, hwnd, NULL, cs->hInstance, NULL);
		data->massSliderMaxLabel = CreateWindow(L"STATIC", L"MSliderMaxLabel", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, 0, 10, 10, hwnd, NULL, cs->hInstance, NULL);
		data->massSliderValueLabel = CreateWindow(L"STATIC", L"MSliderValueLabel", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, 0, 10, 10, hwnd, NULL, cs->hInstance, NULL);

		data->lengthSlider = CreateWindowEx(0, TRACKBAR_CLASS, L"Length", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 0, 0, 10, 10, hwnd, NULL, cs->hInstance, NULL);
		data->lengthSliderNameLabel = CreateWindow(L"STATIC", L"LSliderNameLabel", WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 0, 10, 10, hwnd, NULL, cs->hInstance, NULL);
		data->lengthSliderMinLabel = CreateWindow(L"STATIC", L"LSliderMinLabel", WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 0, 10, 10, hwnd, NULL, cs->hInstance, NULL);
		data->lengthSliderMaxLabel = CreateWindow(L"STATIC", L"LSliderMaxLabel", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, 0, 10, 10, hwnd, NULL, cs->hInstance, NULL);
		data->lengthSliderValueLabel = CreateWindow(L"STATIC", L"LSliderValueLabel", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, 0, 10, 10, hwnd, NULL, cs->hInstance, NULL);
		if (!(data->paintWin && 
			data->massSlider && data->massSliderNameLabel && data->massSliderMinLabel && data->massSliderMaxLabel && data->massSliderValueLabel &&
			data->lengthSlider && data->lengthSliderNameLabel && data->lengthSliderMinLabel && data->lengthSliderMaxLabel && data->lengthSliderValueLabel)) {
			MessageBox(hwnd, L"Unable to create child windows.", L"Error", MB_ICONERROR | MB_OK);
			return -1;
		}

		SendMessage(data->massSlider, TBM_SETRANGE, (WPARAM)TRUE, MAKELPARAM(10, 50));
		SendMessage(data->massSlider, TBM_SETPAGESIZE, 0, (LPARAM)20);
		SendMessage(data->massSlider, TBM_SETLINESIZE, 0, (LPARAM)5);
		SendMessage(data->massSlider, TBM_SETTICFREQ, (WPARAM)5, 0);

		SetWindowText(data->massSliderNameLabel, L"Mass");
		SetWindowText(data->massSliderMinLabel, L"10");
		SetWindowText(data->massSliderMaxLabel, L"50");
		SetWindowText(data->massSliderValueLabel, L"10");

		SendMessage(data->lengthSlider, TBM_SETRANGE, (WPARAM)TRUE, MAKELPARAM(50, 170));
		SendMessage(data->lengthSlider, TBM_SETPAGESIZE, 0, (LPARAM)40);
		SendMessage(data->lengthSlider, TBM_SETLINESIZE, 0, (LPARAM)10);
		SendMessage(data->lengthSlider, TBM_SETTICFREQ, (WPARAM)10, 0);

		SetWindowText(data->lengthSliderNameLabel, L"Length");
		SetWindowText(data->lengthSliderMinLabel, L"50");
		SetWindowText(data->lengthSliderMaxLabel, L"170");
		SetWindowText(data->lengthSliderValueLabel, L"50");

		dialogFont = CreateFont(-12, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Segoe UI");
		if (dialogFont) {
			SendMessage(data->massSliderNameLabel, WM_SETFONT, (WPARAM)dialogFont, FALSE);
			SendMessage(data->massSliderMinLabel, WM_SETFONT, (WPARAM)dialogFont, FALSE);
			SendMessage(data->massSliderMaxLabel, WM_SETFONT, (WPARAM)dialogFont, FALSE);
			SendMessage(data->massSliderValueLabel, WM_SETFONT, (WPARAM)dialogFont, FALSE);

			SendMessage(data->lengthSliderNameLabel, WM_SETFONT, (WPARAM)dialogFont, FALSE);
			SendMessage(data->lengthSliderMinLabel, WM_SETFONT, (WPARAM)dialogFont, FALSE);
			SendMessage(data->lengthSliderMaxLabel, WM_SETFONT, (WPARAM)dialogFont, FALSE);
			SendMessage(data->lengthSliderValueLabel, WM_SETFONT, (WPARAM)dialogFont, FALSE);
		}
		else {
			MessageBox(hwnd, L"Could not create font.", L"Warning", MB_ICONEXCLAMATION | MB_OK);
		}
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
		MoveWindow(data->paintWin, 0, 0, rc.right - 200, rc.bottom, TRUE);

		MoveWindow(data->massSlider, rc.right - 200, 20, 160, 40, FALSE);
		MoveWindow(data->massSliderNameLabel, rc.right - 200, 0, 40, 20, FALSE);
		MoveWindow(data->massSliderMinLabel, rc.right - 200, 60, 40, 20, FALSE);
		MoveWindow(data->massSliderMaxLabel, rc.right - 80, 60, 40, 20, FALSE);
		MoveWindow(data->massSliderValueLabel, rc.right - 40, 20, 35, 20, FALSE);

		MoveWindow(data->lengthSlider, rc.right - 200, 100, 160, 40, FALSE);
		MoveWindow(data->lengthSliderNameLabel, rc.right - 200, 80, 40, 20, FALSE);
		MoveWindow(data->lengthSliderMinLabel, rc.right - 200, 140, 40, 20, FALSE);
		MoveWindow(data->lengthSliderMaxLabel, rc.right - 80, 140, 40, 20, FALSE);
		MoveWindow(data->lengthSliderValueLabel, rc.right - 40, 100, 35, 20, FALSE);
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