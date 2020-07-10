#ifndef CUSTOMTIMER_H
#define CUSTOMTIMER_H

#pragma once
#include <chrono>

class CustomTimer
{
public:
	CustomTimer() noexcept;
	float MarkTime() noexcept;
	float PeekLastMark() const noexcept;
private:
	std::chrono::steady_clock::time_point last;
};

#endif