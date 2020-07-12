#ifndef APP_H
#define APP_H

#pragma once
#include "Window.h"
#include "CustomTimer.h"

class App
{
public:
	App();

	int Start();
private:
	void Update();
	void ShowRawInputData();
	int x = 0, y = 0;

	Window wnd;
	CustomTimer timer;
};

#endif