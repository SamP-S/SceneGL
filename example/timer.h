#pragma once

#include <chrono>

class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> startTime;
public:
	void ResetTimer()
	{
		startTime = std::chrono::high_resolution_clock::now();
	}
	float GetTime()
	{
		std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - startTime;
		return duration.count();
	}
};