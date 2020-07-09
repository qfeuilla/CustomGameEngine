#ifndef KEYBORD_H
#define KEYBORD_H

#pragma once
#include <queue>
#include <bitset>

class Keyboard {
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			Press,
			Release,
			Invalid
		};

		Event() : type(Type::Invalid), code(0u) {}
		Event(Type type, unsigned char code) : type(type), code(code) {}
		bool IsPress() const noexcept {
			return (type == Type::Press);
		}
		bool isRelease() const noexcept {
			return (type == Type::Release);
		}
		bool isInvalid() const noexcept {
			return (type != Type::Invalid);
		}
		unsigned char GetCode() const noexcept {
			return code;
		}
	private:
		Type type;
		unsigned char code;
	};

	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	// Key event
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void CleanKey() noexcept;
	// Char event
	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void CleanChar() noexcept;
	void Clean() noexcept;
	// Constrol autorepeat
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutorepeatIsEnable() const noexcept;
private:
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
	static constexpr unsigned int n_keys = 256u;
	static constexpr unsigned int buffer_size = 16u;
	bool auto_repeatEnable = false;
	// List of all the key with bool to know if enable or disable
	std::bitset<n_keys> key_states;
	std::queue<Event> key_buffer;
	std::queue<char> char_buffer;
};

#endif