#include <windows.h>

#include "MainWin.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstnace, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;

	if (!RegisterMainWin(hInstance)) {
		MessageBox(NULL, L"Window Registration failed", L"Error", MB_ICONERROR | MB_OK);
		return -1;
	}

	hwnd = CreateWindow(MAINWINCLASS, L"Main Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);
	if (!hwnd) {
		MessageBox(NULL, L"Window Creation failed", L"Error", MB_ICONERROR | MB_OK);
		return -1;
	}

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return msg.wParam;
}