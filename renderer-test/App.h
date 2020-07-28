#ifndef APP_H
#define APP_H

#pragma once
#include "Window.h"
#include "CustomTimer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include <set>
#include "Mesh.h"
#include "TestPlane.h"

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

	Window wnd;
	CustomTimer timer;
	float sim_speed = 1.0f;
	PointLight light;
	Camera cam;
	TestPlane bluePlane{ wnd.Gfx(),6.0f,{ 0.3f,0.3f,1.0f,0.0f } };
	TestPlane redPlane{ wnd.Gfx(),6.0f,{ 1.0f,0.3f,0.3f,0.0f } };
	Model test;
};

#endif