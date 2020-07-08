#include "WndException.h"
#include <sstream>

WndException::WndException(int line, const char* file) noexcept : line(line), file(file) {}

const char* WndException::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	what_buffer = oss.str();
	return what_buffer.c_str();
}

const char* WndException::GetType() const noexcept {
	return "Custom Exception";
}

int WndException::GetLine() const noexcept {
	return line;
}

const std::string& WndException::GetFile() const noexcept {
	return file;
}

std::string WndException::GetOriginString() const noexcept {
	std::ostringstream oss;
	oss << "[FILE] : " << file << std::endl
		<< "[LINE] : " << line;
	return oss.str();
}