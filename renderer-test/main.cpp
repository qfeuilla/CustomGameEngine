#include <Windows.h>
#include "WindowsMessageMap.h"

// Handle the behavior of the window
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	/* Debug WM Messages
	static WindowsMessageMap wmm;
	OutputDebugString(wmm(msg, lParam, wParam).c_str());
	*/
#if DEBUG
	static WindowsMessageMap wmm;
	OutputDebugString(wmm(msg, lParam, wParam).c_str());
#endif

	switch (msg)
	{
	// Post a Quit Message and quit Application
	case WM_CLOSE:
		PostQuitMessage(42);
		break;

	case WM_KEYDOWN:
		if (wParam == 'F') {
			SetWindowText(hWnd, "AAAAAA");
		}
		break;

	case WM_KEYUP:
		if (wParam == 'F') {
			SetWindowText(hWnd, "LELUJHA");
		}
		break;
	}

	// Default Window Porcedure
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Main for window App
int CALLBACK WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow) {

	const auto pClassName = "renderer";

	// Register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	// Create the window
	HWND hWnd = CreateWindowEx(
		0,
		pClassName,
		"The Window",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200,
		640, 480,
		nullptr, nullptr,
		hInstance,
		nullptr
	);

	// Show the Window
	ShowWindow(hWnd, 5);
	
	// Message pump
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	// Handle Error return
	if (gResult == -1) {
		return -1;
	}
	else {
		return msg.wParam;
	}
}