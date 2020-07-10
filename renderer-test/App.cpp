#include "App.h"
#include <sstream>

App::App() : wnd(960, 540, "3D Game Engine") { }

int App::Start() {
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		Update();
	}

	if (gResult == -1) {
		throw CHWND_LAST_EXCEPT();
	}

	return msg.wParam;
}

void App::Update() {
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