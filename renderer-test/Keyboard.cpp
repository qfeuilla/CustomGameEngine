#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept {
	return key_states[keycode];
}

Keyboard::Event	Keyboard::ReadKey() noexcept {
	if (key_buffer.size() > 0u) {
		Keyboard::Event e = key_buffer.front();
		key_buffer.pop();
		return e;
	}
	else {
		return Keyboard::Event();
	}
}

bool Keyboard::KeyIsEmpty() const noexcept {
	return key_buffer.empty();
}

char Keyboard::ReadChar() noexcept {
	if (char_buffer.size() > 0u) {
		unsigned char c = char_buffer.front();
		char_buffer.pop();
		return c;
	}
	else {
		return '\0';
	}
}

bool Keyboard::CharIsEmpty() const noexcept {
	return char_buffer.empty();
}

void Keyboard::CleanChar() noexcept {
	char_buffer = std::queue<char>();
}

void Keyboard::CleanKey() noexcept {
	key_buffer = std::queue<Event>();
}

void Keyboard::Clean() noexcept {
	CleanChar();
	CleanKey();
}

void Keyboard::DisableAutorepeat() noexcept {
	auto_repeatEnable = false;
}

void Keyboard::EnableAutorepeat() noexcept {
	auto_repeatEnable = false;
}

bool Keyboard::AutorepeatIsEnable() const noexcept {
	return auto_repeatEnable;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept {
	key_states[keycode] = true;
	key_buffer.push(Event(Event::Type::Press, keycode));
	TrimBuffer(key_buffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept {
	key_states[keycode] = false;
	key_buffer.push(Event(Event::Type::Release, keycode));
	TrimBuffer(key_buffer);
}

void Keyboard::OnChar(char charac) noexcept {
	char_buffer.push(charac);
	TrimBuffer(char_buffer);
}

void Keyboard::ClearState() noexcept {
	key_states.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept {
	while (buffer.size() > buffer_size) {
		buffer.pop();
	}
}