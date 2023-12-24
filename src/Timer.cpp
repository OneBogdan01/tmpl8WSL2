#include "Timer.h"

Timer::Timer()
{
	reset();
}

float Timer::elapsed() const
{
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(t2 - start);
	return time_span.count();
}

void Timer::reset()
{
	start = std::chrono::high_resolution_clock::now();
}
