#include "CustomTimer.h"

using namespace std::chrono;

CustomTimer::CustomTimer() noexcept {
	last = steady_clock::now();
}

float CustomTimer::MarkTime() noexcept {
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float CustomTimer::PeekLastMark() const noexcept {
	return duration<float>(steady_clock::now() - last).count();
}