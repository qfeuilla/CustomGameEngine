#include "App.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "CustomMath.h"
#include <memory>
#include "Surface.h"
#include "imgui/imgui.h"
#include "VertexBuffer.h"
#include <dxtex/DirectXTex.h>
#include "Testing.h"
#include "PerfLog.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "DynamicConstant.h"

namespace dx = DirectX;

App::App()
	:
	wnd((int)WIDTH, (int)HEIGHT, "The Donkey Fart Box"),
	light(wnd.Gfx()),
	gobber(wnd.Gfx(), "Models\\gobber\\GoblinX.obj", 6.0f)
{
	//TestMaterialSystemLoading(wnd.Gfx());
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, HEIGHT / WIDTH, 0.5f, 4000.0f));
	/*
	cube.SetPos({ 4.0f,0.0f,0.0f });
	cube2.SetPos({ 0.0f,4.0f,0.0f });
	{
		std::string path = "Models\\brick_wall\\brick_wall.obj";
		Assimp::Importer imp;
		const auto pScene = imp.ReadFile(path,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ConvertToLeftHanded |
			aiProcess_GenNormals |
			aiProcess_CalcTangentSpace
		);
		Material mat{ wnd.Gfx(),*pScene->mMaterials[1],path };
		pLoaded = std::make_unique<Mesh>(wnd.Gfx(), mat, *pScene->mMeshes[0]);
	}
	*/
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

	light.Submit(fc);
	gobber.Submit(fc);
	//test.Draw(wnd.Gfx());
	//cube.Draw(wnd.Gfx());
	//cube2.Draw(wnd.Gfx());
	//cube.DrawOutline(wnd.Gfx());
	//cube2.DrawOutline(wnd.Gfx());
	//cube.Submit(fc);
	//cube2.Submit(fc);
	//pLoaded->Submit(fc, DirectX::XMMatrixIdentity());

	fc.Execute(wnd.Gfx());

	// User Inputs
	Controls(dt);

	// Mesh techniques window
	class Probe : public TechniqueProbe
	{
	public:
		void OnSetTechnique() override
		{
			using namespace std::string_literals;
			ImGui::TextColored({ 0.4f,1.0f,0.6f,1.0f }, pTech->GetName().c_str());
			bool active = pTech->IsActive();
			ImGui::Checkbox(("Tech Active##"s + std::to_string(techIdx)).c_str(), &active);
			pTech->SetActiveState(active);
		}
		bool OnVisitBuffer(dynamical::Buffer& buf) override
		{
			namespace dx = DirectX;
			float dirty = false;
			const auto dcheck = [&dirty](bool changed) {dirty = dirty || changed; };
			auto tag = [tagScratch = std::string{}, tagString = "##" + std::to_string(bufIdx)]
			(const char* label) mutable
			{
				tagScratch = label + tagString;
				return tagScratch.c_str();
			};

			if (auto v = buf["scale"]; v.Exists())
			{
				dcheck(ImGui::SliderFloat(tag("Scale"), &v, 1.0f, 2.0f, "%.3f", 3.5f));
			}
			if (auto v = buf["materialColor"]; v.Exists())
			{
				dcheck(ImGui::ColorPicker3(tag("Color"), reinterpret_cast<float*>(&static_cast<dx::XMFLOAT3&>(v))));
			}
			if (auto v = buf["specularColor"]; v.Exists())
			{
				dcheck(ImGui::ColorPicker3(tag("Spec. Color"), reinterpret_cast<float*>(&static_cast<dx::XMFLOAT3&>(v))));
			}
			if (auto v = buf["specularGloss"]; v.Exists())
			{
				dcheck(ImGui::SliderFloat(tag("Glossiness"), &v, 1.0f, 100.0f, "%.1f", 1.5f));
			}
			if (auto v = buf["specularWeight"]; v.Exists())
			{
				dcheck(ImGui::SliderFloat(tag("Spec. Weight"), &v, 0.0f, 2.0f));
			}
			if (auto v = buf["useNormalMap"]; v.Exists())
			{
				dcheck(ImGui::Checkbox(tag("Normal Map Enable"), &v));
			}
			if (auto v = buf["normalMapWeight"]; v.Exists())
			{
				dcheck(ImGui::SliderFloat(tag("Normal Map Weight"), &v, 0.0f, 2.0f));
			}
			return dirty;
		}
	} probe;
	//pLoaded->Accept(probe);

	// imgui windows
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	FPSCounter();
	//cube.SpawnControlWindow(wnd.Gfx(), "Cube 1");
	//cube2.SpawnControlWindow(wnd.Gfx(), "Cube 2");
	//gobber.ShowWindow(wnd.Gfx(), "Test obj");

	// present
	wnd.Gfx().EndFrame();
	fc.Reset();
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