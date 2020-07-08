#ifndef WNDEXCEPTION_H
#define WNDEXCEPTION_H

#pragma once
#include <exception>
#include <string>

class WndException : public std::exception {
public:
	WndException(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;

private:
	// Line and File where error is shout
	int line;
	std::string file;

protected:
	mutable std::string what_buffer;
};

#endif