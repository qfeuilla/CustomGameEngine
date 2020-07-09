#include "Mouse.h"

std::pair<int, int> Mouse::GetPos() const noexcept {
	return { x, y };
}

int Mouse::GetPosX() const noexcept {
	return x;
}

int Mouse::GetPosY() const noexcept {
	return y;
}

bool Mouse::LeftIsPressed() const noexcept {
	return l_pressed;
}

bool Mouse::RightIsPressed() const noexcept {
	return r_pressed;
}

bool Mouse::WheelIsPressed() const noexcept {
	return w_pressed;
}

std::optional<Mouse::Event>	Mouse::Read() noexcept {
	if (buffer.size() > buffer_size) {
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	return {};
}

bool Mouse::IsEmty() const noexcept {
	return buffer.empty();
}

void Mouse::Clean() noexcept {
	buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int newx, int newy) noexcept {
	x = newx;
	y = newy;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int newx, int newyy) noexcept {
	l_pressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftRelease(int newx, int newy) noexcept {
	l_pressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int newx, int newy) noexcept {
	r_pressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightRelease(int newx, int newy) noexcept {
	r_pressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelPressed(int newx, int newy) noexcept {
	w_pressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::WPress, *this));
	TrimBuffer();
}

void Mouse::OnWheelRelease(int newx, int newy) noexcept {
	w_pressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::WRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int newx, int newy) noexcept {
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int newx, int newy) noexcept {
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept {
	while (buffer.size() > buffer_size)
	{
		buffer.pop();
	}
}