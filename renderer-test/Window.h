
#ifndef WINDOW_H
#define WINDOW_H

#pragma once
#include "CustomWindows.h"

class Window
{
public:
	Window(int width, int height, const char * name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

private:
	// singleton manages registration/cleanup of window class
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "DirectX CPP Engine";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

	// Msg Handlers
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	// Window Parameters
	int width;
	int height;
	HWND hWnd;
};

#endif