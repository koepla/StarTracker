#ifndef STARTRACKER_CORE_LOGGER_H
#define STARTRACKER_CORE_LOGGER_H

#include <StarEngine/DateTime.hpp>

#include <fmt/format.h>

#include <vector>
#include <mutex>
#include <string_view>

namespace StarTracker::Core {

	class Logger {

	private:
		struct LoggerData {

			std::mutex Mutex;
			std::vector<std::string> List;
		};

		static inline LoggerData info{};
		static inline LoggerData warn{};
		static inline LoggerData error{};

	public:
		[[nodiscard]] static bool Info(std::string_view message) noexcept;
		[[nodiscard]] static bool Warn(std::string_view message) noexcept;
		[[nodiscard]] static bool Error(std::string_view message) noexcept;

		static void ClearInfoList() noexcept;
		static void ClearWarnList() noexcept;
		static void ClearErrorList() noexcept;

		[[nodiscard]] static std::vector<std::string> GetInfoList() noexcept;
		[[nodiscard]] static std::vector<std::string> GetWarnList() noexcept;
		[[nodiscard]] static std::vector<std::string> GetErrorList() noexcept;

		[[nodiscard]] static std::size_t GetInfoSize() noexcept;
		[[nodiscard]] static std::size_t GetWarnSize() noexcept;
		[[nodiscard]] static std::size_t GetErrorSize() noexcept;

	private:
		[[nodiscard]] static std::string formatEntry(std::string_view entry) noexcept;
	};
}

#define STARTRACKER_INFO(...) (void)StarTracker::Core::Logger::Info(fmt::format(__VA_ARGS__)) 
#define STARTRACKER_WARN(...) (void)StarTracker::Core::Logger::Warn(fmt::format(__VA_ARGS__)) 
#define STARTRACKER_ERROR(...) (void)StarTracker::Core::Logger::Error(fmt::format(__VA_ARGS__)) 

#endif // STARTRACKER_CORE_LOGGER_H