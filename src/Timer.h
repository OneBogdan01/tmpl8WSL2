#pragma once
#include <chrono>

// FPSTimer

struct Timer
{
	Timer();

	float elapsed() const;

	void reset();

	std::chrono::high_resolution_clock::time_point start;
};
