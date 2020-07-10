#include "App.h"
#include <sstream>
#include <iomanip>

App::App() : wnd(960, 540, "3D Game Engine") { }

int App::Start() {
	while (true) {
		// Test if exit code
		if (const auto ecode = Window::ProcessMessages()) {
			return *ecode;
		}
		Update();
	}
}

void App::Update() {
	wnd.Gfx().EndFrame();
}