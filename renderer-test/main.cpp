#include "Window.h"
#include <sstream>

// Main for window App
int CALLBACK WinMain(
	HINSTANCE	h_instance,
	HINSTANCE	h_prev_instance,
	LPSTR		lp_cmd_line,
	int			n_cmd_show) {
	try {
		Window wnd(1920, 1080, "3D Game Engine");

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			
			while (!wnd.mouse.IsEmty()) {
				const auto e = wnd.mouse.Read();
				switch (e.GetType()) {
				case Mouse::Event::Type::Leave:
					wnd.SetTitle("Gone !");
					break;

				case Mouse::Event::Type::Move:
					std::ostringstream oss;
					oss << "Mouse move to {" << wnd.mouse.GetPosX() << ", " << wnd.mouse.GetPosY() << "}";
					wnd.SetTitle(oss.str());
					break;

				}
			}
			
		}

		if (gResult == -1) {
			return -1;
		}

		return msg.wParam;
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