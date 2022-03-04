#ifndef STARTRACKER_UTILS_STOPWATCH_H
#define STARTRACKER_UTILS_STOPWATCH_H

#include <chrono>

namespace StarTracker::Utils::Diagnostics {

	class Stopwatch {

	private:
		bool running;
		std::chrono::steady_clock::time_point startTime;
		std::chrono::steady_clock::time_point stopTime;

	public:
		Stopwatch() noexcept;

		/**
		* Starts the stopwatch
		* 
		*/
		void Start() noexcept;

		/**
		* Stops the stopwatch
		*
		*/
		void Stop() noexcept;

		/**
		* Number of milliseconds that are ellapsed since start or the timespan between stop and start
		* 
		* @return number of ellapsed milliseconds
		* 
		*/
		[[nodiscard]] double GetEllapsedMilliseconds() const noexcept;
	};
}

#endif // STARTRACKER_UTILS_STOPWATCH_H