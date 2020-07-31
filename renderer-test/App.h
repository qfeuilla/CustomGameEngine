#ifndef APP_H
#define APP_H

#pragma once
#include "Window.h"
#include "CustomTimer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include <set>
#include "Model.h"
#include "TestPlane.h"
#include "TestCube.h"
#include "FrameCommander.h"
#include "Material.h"

class App
{
public:
	App();

	int Start();
	~App();
private:
	ImguiManager imgui;
	void Update();
	void ShowImguiDemoWindow();
	void FPSCounter();
	void Controls(float);
	void FPSMouseControl();
	void FPSKeybrdControl(float);
	bool showDemoWindow = false;

	FrameCommander fc;
	Window wnd;
	CustomTimer timer;
	float sim_speed = 1.0f;
	PointLight light;
	Camera cam;
	Model gobber;
	//TestCube cube{ wnd.Gfx(),4.0f };
	//TestCube cube2{ wnd.Gfx(),4.0f };
	//std::unique_ptr<Mesh> pLoaded;
};

#endif