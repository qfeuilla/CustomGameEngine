#include "App.h"
#include <sstream>
#include <iomanip>
#include <iostream>

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
	wnd.Gfx().ClearBuffer(0.7f, 0.7f, 1.0f);
	wnd.Gfx().DrawTestTriangle(
		-timer.PeekLastMark(),
		0.0f,
		0.0f
	);
	wnd.Gfx().DrawTestTriangle(timer.PeekLastMark(), 
		(wnd.mouse.GetPosX() / (wnd.width / 2)) - 1,
		-((wnd.mouse.GetPosY() / (wnd.height / 2)) - 1));
	ShowRawInputData();
	wnd.Gfx().EndFrame();
}

void App::ShowRawInputData() {
	if (wnd.mouse.RawDeltaIsEmpty()) {
		x = 0;
		y = 0;
	}
	while (const auto d = wnd.mouse.ReadRawDelta()) {
		x = d->x;
		y = d->y;
	}
	std::ostringstream oss;
	oss << "Raw x : " << x << "Raw y : " << y;
	auto title = oss.str();
	wnd.SetTitle(title.c_str());
}
