#include "App.h"
#include <sstream>
#include <iomanip>

App::App() : wnd(800, 600, "3D Game Engine") { }

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
	wnd.Gfx().ClearBuffer(0.0f, 0.0f, 1.0f);
	wnd.Gfx().DrawTestTriangle();
	wnd.Gfx().EndFrame();
}