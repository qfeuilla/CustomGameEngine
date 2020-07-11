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
	const float c = sin(timer.PeekLastMark()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().EndFrame();
}