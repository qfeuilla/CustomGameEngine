#ifndef MOUSE_H
#define MOUSE_H

#pragma once
#include <queue>
#include <optional>

class Mouse
{
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			LPress,
			LRelease,
			RPress,
			RRelease,
			WPress,
			WRelease,
			WheelUp,
			WheelDown,
			Move,
			Invalid
		};

		Event(Type type, const Mouse& parent) noexcept :
			type(type),
			l_pressed(parent.l_pressed),
			r_pressed(parent.r_pressed),
			w_pressed(parent.w_pressed),
			x(parent.x),
			y(parent.y) {}

		bool isValid() {
			return type != Type::Invalid;
		}
		Type GetType() const noexcept {
			return type;
		}
		std::pair<int, int> GetPos() const noexcept {
			return { x, y };
		}
		int GetPosX() const noexcept {
			return x;
		}
		int GetPosY() const noexcept {
			return y;
		}
		bool LeftIsPressed() const noexcept {
			return l_pressed;
		}
		bool RightIsPressed() const noexcept {
			return r_pressed;
		}
		bool WheelIsPressed() const noexcept {
			return w_pressed;
		}

	private:
		Type type;
		bool l_pressed;
		bool r_pressed;
		bool w_pressed;
		int x;
		int y;
	};

	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	bool WheelIsPressed() const noexcept;
	std::optional<Mouse::Event> Read() noexcept;
	bool IsEmty() const noexcept;
	void Clean() noexcept;

private:
	void OnMouseMove(int x, int y) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftRelease(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightRelease(int x, int y) noexcept;
	void OnWheelPressed(int x, int y) noexcept;
	void OnWheelRelease(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;

	static constexpr unsigned int buffer_size = 16u;
	int x;
	int y;
	bool l_pressed = false;
	bool r_pressed = false;
	bool w_pressed = false;
	std::queue<Event> buffer;
};

#endif