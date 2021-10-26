#include "Stopwatch.hpp"

namespace Utils::Diagnostics {

	Stopwatch::Stopwatch() noexcept : running(false), startTime(), stopTime() {


	}

	void Stopwatch::Start() noexcept {

		startTime = std::chrono::high_resolution_clock::now();
		running = true;
	}

	void Stopwatch::Stop() noexcept {

		stopTime = std::chrono::high_resolution_clock::now();
		running = false;
	}

	double Stopwatch::GetEllapsedMilliseconds() const noexcept {

		if (running) {

			return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count());
		}
		else {

			return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime).count());
		}
	}
}