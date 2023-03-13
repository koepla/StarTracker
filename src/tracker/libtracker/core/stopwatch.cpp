#include "stopwatch.hpp"

Stopwatch::Stopwatch() noexcept : running{ false } { }

void Stopwatch::Start() noexcept {
    startTime = std::chrono::high_resolution_clock::now();
    running = true;
}

void Stopwatch::Stop() noexcept {
    stopTime = std::chrono::high_resolution_clock::now();
    running = false;
}

f64 Stopwatch::GetElapsedMilliseconds() const noexcept {
    if (running) {
        return static_cast<f64>(std::chrono::duration_cast<std::chrono::microseconds>(
                                        std::chrono::high_resolution_clock::now() - startTime)
                                        .count()) /
               1000.0;
    }
    return static_cast<f64>(std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime).count()) /
           1000.0;
}
