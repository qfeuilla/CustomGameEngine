#include "Window.h"
#include <sstream>
#include "resource.h"
#include "WindowsMessageMap.h"
#include "WindowsThrowMacros.h"

// Window Class Stuff
Window::WindowClass Window::WindowClass::wnd_class;

Window::WindowClass::WindowClass() noexcept : h_inst(GetModuleHandle(nullptr)) {
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(
		GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 128, 128, 0
	));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(
		GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 64, 64, 0
	));	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
	UnregisterClass(GetName(), GetInstance());
}

const char* Window::WindowClass::GetName() noexcept {
	return wnd_class_name;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept {
	return wnd_class.h_inst;
}

// Window Stuff
Window::Window(int width, int height, const char* name) : width(width), height(height) {
	// Calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0) {
		throw CHWND_LAST_EXCEPT();
	}

	// Create window & get hWnd
	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	// Check for creation window error
	if (hWnd == nullptr) {
		throw CHWND_LAST_EXCEPT();
	}

	// Newly created windows start off as hidden
	ShowWindow(hWnd, SW_SHOWDEFAULT);

	pGfx = std::make_unique<Graphics>(hWnd);

	// Register Mouse as raw input device
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE) {
		throw CHWND_LAST_EXCEPT();
	}
}

Window::~Window() {
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::string& title) noexcept {
	if (SetWindowText(hWnd, title.c_str()) == 0) {
		throw CHWND_LAST_EXCEPT();
	}
}

std::optional<int> Window::ProcessMessages() noexcept {
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		// Check for quit message because peekmessage does not signal via return
		if (msg.message == WM_QUIT) {
			return (int)msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Return empty otional when not quitting app
	return {};
}

Graphics& Window::Gfx()
{
	if (!pGfx) {
		throw CHWND_NOGFX_EXCEPT();
	}
	return *pGfx;
}

// Only used to setup the Window
LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	// Use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// Extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// Set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// Set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// Forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// If we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// The msg handler used after setup
LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	// Retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// Forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	// Debug WM Messages
	/*
	static WindowsMessageMap wmm;
	OutputDebugString(wmm(msg, lParam, wParam).c_str());
	*/
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		// Don't need to destroy windows because custom destructor
		return 0;
		// Clear keystates whent focus changes, prevent input repeat
	case WM_KILLFOCUS:
		keyBrd.ClearState();
		break;
		// Keyboard MESSAGES
	case WM_KEYDOWN:
		// Siskey is used to track ALT key
	case WM_SYSKEYDOWN:
		// Filter multiple key press (like WASD control)
		if (keyBrd.AutorepeatIsEnable() || !(lParam & 0x40000000)) {
			keyBrd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		keyBrd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		keyBrd.OnChar(static_cast<unsigned char>(wParam));
		break;
		// End Keyboard

		// Mouse controll
	case WM_MOUSEMOVE: {
		const POINTS pt = MAKEPOINTS(lParam);

		// If mouse in client region
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height) {
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow()) {
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else {
			// If Dragging
			if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON)) {
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else {
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN: {
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN: {
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP: {
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftRelease(pt.x, pt.y);
		SetForegroundWindow(hWnd);
		break;
	}
	case WM_RBUTTONUP: {
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightRelease(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONDOWN: {
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnWheelPressed(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONUP: {
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnWheelRelease(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL: {
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
					  // End Mouse

					  // Handle Raw Input
	case WM_INPUT: {
		UINT size;

		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			nullptr,
			&size,
			sizeof(RAWINPUTHEADER)) == -1) {
			// Something whent wrong ignore this input
			break;
		}
		raw_buffer.resize(size);
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			raw_buffer.data(),
			&size,
			sizeof(RAWINPUTHEADER)) != size) {
			// Something whent wrong ignore this input
			break;
		}

		auto& ri = reinterpret_cast<const RAWINPUT&>(*raw_buffer.data());
		if (ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0)) {
			mouse.OnRawInputDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
		}
		break;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Window exception stuff
std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept {
	char* pMsgBuf = nullptr;
	// Windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// Copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// Free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept :
	Exception(line, file),
	hr(hr) {}

const char* Window::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	what_buffer = oss.str();
	return what_buffer.c_str();
}

const char* Window::HrException::GetType() const noexcept {
	return "Window Exception";
}

HRESULT Window::HrException::GetErrorCode() const noexcept {
	return hr;
}

std::string Window::HrException::GetErrorDescription() const noexcept {
	return Exception::TranslateErrorCode(hr);
}

const char* Window::NoGfxException::GetType() const noexcept {
	return "Window Exception [No Graphics]";
}