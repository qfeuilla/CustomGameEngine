#ifndef APP_H
#define APP_H

#pragma once
#include "Window.h"

class App
{
public:
	App();

	int Start();
private:
	void Update();

	Window wnd;
};

#endif