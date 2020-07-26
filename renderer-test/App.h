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
	void ShowRawInputData();
	int x = 0, y = 0;

	Window wnd;
	CustomTimer timer;
	float sim_speed = 1.0f;
	PointLight light;
	Camera cam;
	Model nano;
};

#endif