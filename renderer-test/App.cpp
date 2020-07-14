#include "App.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include "Box.h"
#include <memory>

App::App() : wnd(800, 600, "3D Game Engine") {
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 1.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.08f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 180; i++)
	{
		boxes.push_back(std::make_unique<Box>(
			wnd.Gfx(), rng, adist,
			ddist, odist, rdist
			));
	}
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

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
	auto dt = timer.MarkTime();
	wnd.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);
	for (auto& b : boxes)
	{
		b->Update(dt);
		b->Draw(wnd.Gfx());
	}
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

App::~App() {}