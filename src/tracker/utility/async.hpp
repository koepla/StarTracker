#ifndef UTILITY_ASYNC_H
#define UTILITY_ASYNC_H

#include <functional>
#include <thread>

#include "types.hpp"

namespace utility {

    /**
     * Represents a task that is executed asynchronously
     */
    using AsyncTask = std::function<void()>;

    /**
     * Executes the specified task asynchronously, does not inform the caller whether the task has finished execution.
     * Therefore: fire and forget
     * @param task Async task
     */
    void FireAndForget(AsyncTask&& task) noexcept;
}// namespace utility

#endif// UTILITY_ASYNC_H