#ifndef UWU_TIMER
#define UWU_TIMER
#include <iostream>
#include <SDL.h>
#include <chrono>
//Class to act as a timer using the chrono library
class UWU_Timer
{
public:
	UWU_Timer() : _startTime(std::chrono::steady_clock::now()), _lastTime(_startTime) {}
	void Reset() {
		_startTime = std::chrono::steady_clock::now();
		_lastTime = _startTime;
	}
	//Get the time since the last time this was called
	template <typename T = std::chrono::milliseconds>
	long long GetDeltaTicks() {
		long long  deltaTime = std::chrono::duration_cast<T>(std::chrono::steady_clock::now() - _lastTime).count();
		return deltaTime;
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> _startTime;
	std::chrono::time_point<std::chrono::steady_clock> _lastTime;
};

#endif //!UWU_TIMER
