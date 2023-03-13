#ifndef LIBTRACKER_CORE_H
#define LIBTRACKER_CORE_H

#include <mutex>
#include <vector>

#include <fmt/format.h>
#include <libengine/date-time.hpp>

#include "arch/os.hpp"
#include "utility/types.hpp"

class Logger {
public:
    /**
     * Severity of the event causing the log. Note that the severity has no correlation to the state of the program. An
     * error does not mean, that the application is in an invalid state
     */
    enum class Severity { Info, Warn, Error };

    /**
     * Utility struct for storing a severity and a message
     */
    struct Entry {
        Severity Sev;
        std::string Message;
    };

    /**
     * Log with `Info` severity
     * @param message
     */
    static void Info(const std::string& message) noexcept;

    /**
     * Log with `Warning` severity
     * @param message
     */
    static void Warn(const std::string& message) noexcept;

    /**
     * Log with `Error` severity
     * @param message
     */
    static void Error(const std::string& message) noexcept;

    /**
     * Returns the entire log since the program started
     * @return log
     */
    static const std::vector<Entry>& GetLog() noexcept;

private:
    /**
     * Mutex to guarantee thread safety, as we may access the log list from different threads of execution. If a thread
     * is submitting a log message, all of the log variants will halt if another thread tries to call it
     */
    static inline std::mutex Mutex;

    /**
     * List for storing all the log messages
     */
    static inline std::vector<Entry> List;
};

#define LIBTRACKER_INFO(...) (void) Logger::Info(fmt::format(__VA_ARGS__))
#define LIBTRACKER_WARN(...) (void) Logger::Warn(fmt::format(__VA_ARGS__))
#define LIBTRACKER_ERROR(...) (void) Logger::Error(fmt::format(__VA_ARGS__))

#ifdef _DEBUG
#define LIBTRACKER_ASSERT(expr, ...)                                                                    \
    if (!(expr)) {                                                                                      \
        const auto message = fmt::format(__VA_ARGS__);                                                  \
        LIBTRACKER_ERROR("Assertion failed ({}) in file {} on line {}}!", message, __FILE__, __LINE__); \
        __debugbreak();                                                                                 \
        abort();                                                                                        \
    }
#else
#define LIBTRACKER_ASSERT(expr, ...) (void) (expr)
#endif

#endif// LIBTRACKER_CORE_H
