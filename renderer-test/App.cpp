#include "App.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include "Box.h"
#include <algorithm>
#include "CustomMath.h"
#include <memory>
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"

GDIPlusManager gdip_man;

App::App()
	:
	wnd(WIDTH, HEIGHT, "The Donkey Fart Box"),
	light(wnd.Gfx())
{
	class Factory
	{
	public:
		Factory(Graphics& gfx)
			:
			gfx(gfx)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			return std::make_unique<Box>(
				gfx, rng, adist, ddist,
				odist, rdist, bdist
				);
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
	};

	Factory f(wnd.Gfx());
	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, f);
	// wnd.Gfx().SetCamera(DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
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
	auto dt = timer.MarkTime() * sim_speed;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx());

	for (auto& d : drawables)
	{
		d->Update(wnd.keyBrd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
		d->Draw(wnd.Gfx());
	}
	light.Draw(wnd.Gfx());

	if (ImGui::Begin("Simulation Speed")) {
		ImGui::SliderFloat("Speed Factor", &sim_speed, 0.0f, 20.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();

	// imgui window to control camera
	cam.SpawnControlWindow();
	light.SpawnControlWindow();

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