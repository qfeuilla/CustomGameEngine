#include "App.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "CustomMath.h"
#include <memory>
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"

GDIPlusManager gdip_man;

namespace dx = DirectX;

App::App()
	:
	wnd(WIDTH, HEIGHT, "The Donkey Fart Box"),
	light(wnd.Gfx()),
	nano(wnd.Gfx(), "Models\\nano_hierarchy.gltf")
{
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, HEIGHT / WIDTH, 0.5f, 80.0f));
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
	light.Bind(wnd.Gfx(), cam.GetMatrix());
	nano.Draw(wnd.Gfx());	
	light.Draw(wnd.Gfx());


	while (const auto e = wnd.keyBrd.ReadKey())
	{
		if (e->IsPress() && e->GetCode() == VK_INSERT)
		{
			if (wnd.CursorEnabled()) {
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
		}
	}

	// imgui windows
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	nano.ShowWindow();
	ShowRawInputWindow();
	// present
	wnd.Gfx().EndFrame();
}

void App::ShowImguiDemoWindow()
{
	static bool show_demo_window = true;
	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}
}

void App::ShowRawInputWindow()
{
	while (const auto d = wnd.mouse.ReadRawDelta())
	{
		x += d->x;
		y += d->y;
	}
	if (ImGui::Begin("Raw Input"))
	{
		ImGui::Text("Tally: (%d,%d)", x, y);
		ImGui::Text("Cursor: %s", wnd.CursorEnabled() ? "enabled" : "disabled");
	}
	ImGui::End();
}

App::~App() {}