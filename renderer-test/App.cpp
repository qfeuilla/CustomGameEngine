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
#include "VertexBuffer.h"

GDIPlusManager gdip_man;

namespace dx = DirectX;

App::App()
	:
	wnd((int)WIDTH, (int)HEIGHT, "The Donkey Fart Box"),
	light(wnd.Gfx()),
	muro(wnd.Gfx(), "Models\\muro\\muro.obj", 10.0f),
	nano(wnd.Gfx(), "Models\\nano_textured\\nanosuit.obj", 2.0f),
	wall1(wnd.Gfx(), "Models\\brick_wall\\brick_wall.obj", 6.0f),
	wall2(wnd.Gfx(), "Models\\brick_wall\\brick_wall.obj", 6.0f),
	gobber(wnd.Gfx(), "Models\\gobber\\GoblinX.obj", 6.0f)
{
	wall1.SetRootTransform(dx::XMMatrixTranslation(-12.0f, 0.0f, 0.0f));
	wall2.SetRootTransform(dx::XMMatrixTranslation(12.0f, 0.0f, 0.0f));
	gobber.SetRootTransform(dx::XMMatrixTranslation(0.0f, 0.0f, -4.0f));
	nano.SetRootTransform(dx::XMMatrixTranslation(0.0f, -7.0f, 6.0f));
	muro.SetRootTransform(dx::XMMatrixTranslation(0.0f, 7.0f, -6.0f));
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

void App::FPSKeybrdControl(float dt) {
	if (!wnd.CursorEnabled())
	{
		if (wnd.keyBrd.KeyIsPressed('Z'))
		{
			cam.Translate({ 0.0f,0.0f,dt });
		}
		if (wnd.keyBrd.KeyIsPressed('Q'))
		{
			cam.Translate({ -dt,0.0f,0.0f });
		}
		if (wnd.keyBrd.KeyIsPressed('S'))
		{
			cam.Translate({ 0.0f,0.0f,-dt });
		}
		if (wnd.keyBrd.KeyIsPressed('D'))
		{
			cam.Translate({ dt,0.0f,0.0f });
		}
		if (wnd.keyBrd.KeyIsPressed('E'))
		{
			cam.Translate({ 0.0f,dt,0.0f });
		}
		if (wnd.keyBrd.KeyIsPressed('A'))
		{
			cam.Translate({ 0.0f,-dt,0.0f });
		}
	}
}

void App::FPSMouseControl() {
	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			cam.Rotate((float)delta->x, (float)delta->y);
		}
	}
}

void App::Controls(float dt) {

	while (const auto e = wnd.keyBrd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		case VK_F1:
			showDemoWindow = true;
			break;
		}
	}

	FPSKeybrdControl(dt);
	FPSMouseControl();
}

void App::Update() {
	auto dt = timer.MarkTime() * sim_speed;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(), cam.GetMatrix());
	muro.Draw(wnd.Gfx());
	wall1.Draw(wnd.Gfx());
	wall2.Draw(wnd.Gfx());
	nano.Draw(wnd.Gfx());
	gobber.Draw(wnd.Gfx());
	light.Draw(wnd.Gfx());

	// User Inputs
	Controls(dt);

	// imgui windows
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	FPSCounter();
	muro.ShowWindow(wnd.Gfx(), "Muro");
	wall1.ShowWindow(wnd.Gfx(), "Wall 1");
	wall2.ShowWindow(wnd.Gfx(), "Wall 2");
	gobber.ShowWindow(wnd.Gfx(), "Gobber");
	nano.ShowWindow(wnd.Gfx(), "nanosuit");

	// present
	wnd.Gfx().EndFrame();
}

void App::ShowImguiDemoWindow()
{
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
}

void App::FPSCounter()
{
	if (ImGui::Begin("FPS Counter : ")) {
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	}
	ImGui::End();
}

App::~App() {}