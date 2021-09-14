#ifndef _STOPWATCH_H_
#define _STOPWATCH_H_

#include <chrono>

namespace Utils::Diagnostics {

	class Stopwatch {

	private:
		bool running;
		std::chrono::steady_clock::time_point startTime;
		std::chrono::steady_clock::time_point stopTime;

	public:
		Stopwatch();

		void Start() noexcept;
		void Stop() noexcept;

		double GetEllapsedMilliseconds() const noexcept;
	};
}

#endif // _STOPWATCH_H_

