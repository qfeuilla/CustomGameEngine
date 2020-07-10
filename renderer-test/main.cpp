#include "App.h"

// Main for window App
int CALLBACK WinMain(
	HINSTANCE	h_instance,
	HINSTANCE	h_prev_instance,
	LPSTR		lp_cmd_line,
	int			n_cmd_show) {
	try {
		return App{}.Start();
	}
	catch (const WndException& e) {
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		MessageBox(nullptr, "No details", "Unknow Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}