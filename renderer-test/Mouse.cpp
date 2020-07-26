#include "Mouse.h"
#include <Windows.h>

std::pair<int, int> Mouse::GetPos() const noexcept {
	return { x, y };
}

float Mouse::GetPosX() const noexcept {
	return x;
}

float Mouse::GetPosY() const noexcept {
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

bool Mouse::IsInWindow() const noexcept {
	return isInWindow;
}

Mouse::Event	Mouse::Read() noexcept {
	if (buffer.size() > 0u) {
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	return Mouse::Event();
}

std::optional<Mouse::RawDelta> Mouse::ReadRawDelta() noexcept
{
	if (rawDeltaBuffer.empty())
	{
		return std::nullopt;
	}
	const RawDelta d = rawDeltaBuffer.front();
	rawDeltaBuffer.pop();
	return d;
}

bool Mouse::IsEmty() const noexcept {
	return buffer.empty();
}

void Mouse::Clean() noexcept {
	buffer = std::queue<Event>();
}

void Mouse::EnableRaw() noexcept
{
	rawEnabled = true;
}

void Mouse::DisableRaw() noexcept
{
	rawEnabled = false;
}

bool Mouse::RawEnabled() const noexcept
{
	return rawEnabled;
}

void Mouse::OnMouseMove(int newx, int newy) noexcept {
	x = newx;
	y = newy;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept {
	isInWindow = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept {
	isInWindow = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
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

void Mouse::TrimRawInputBuffer() noexcept {
	while (rawDeltaBuffer.size() > buffer_size)
	{
		rawDeltaBuffer.pop();
	}
}



void Mouse::OnWheelDelta(int x, int y, int delta) noexcept {
	wheel_delta_carry += delta;
	// Generate events for fast wheel rotation
	// (break down one big events in multiple normal ones)
	while (wheel_delta_carry >= WHEEL_DELTA) {
		wheel_delta_carry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheel_delta_carry <= -WHEEL_DELTA) {
		wheel_delta_carry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}

void Mouse::OnRawDelta(int dx, int dy) noexcept
{
	rawDeltaBuffer.push({ dx,dy });
	TrimBuffer();
}