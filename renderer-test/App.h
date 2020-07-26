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
	void ShowModelWindow();
	void ShowRawInputData();
	int x = 0, y = 0;

	Window wnd;
	CustomTimer timer;
	float sim_speed = 1.0f;
	PointLight light;
	Camera cam;
	Model nano;
	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;
};

#endif