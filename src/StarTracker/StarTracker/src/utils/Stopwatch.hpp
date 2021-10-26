#ifndef STARTRACKER_UTILS_STOPWATCH_H
#define STARTRACKER_UTILS_STOPWATCH_H

#include <chrono>

namespace Utils::Diagnostics {

	class Stopwatch {

	private:
		bool running;
		std::chrono::steady_clock::time_point startTime;
		std::chrono::steady_clock::time_point stopTime;

	public:
		Stopwatch() noexcept;

		void Start() noexcept;
		void Stop() noexcept;

		double GetEllapsedMilliseconds() const noexcept;
	};
}

#endif // STARTRACKER_UTILS_STOPWATCH_H

