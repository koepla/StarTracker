#ifndef LIBTRACKER_UTILS_STOPWATCH_H
#define LIBTRACKER_UTILS_STOPWATCH_H

#include <chrono>
#include <functional>

#include "utility/types.hpp"

class Stopwatch {
public:
    Stopwatch() noexcept;

    /**
     * @brief Starts the stopwatch
     */
    void Start() noexcept;

    /**
     * @brief Stops the stopwatch
     */
    void Stop() noexcept;

    /**
     * @brief Number of milliseconds that are elapsed since start or the timespan between stop and start
     * @return number of elapsed milliseconds
     */
    f64 GetElapsedMilliseconds() const noexcept;

private:
    bool running;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point stopTime;
};

#endif// LIBTRACKER_UTILS_STOPWATCH_H