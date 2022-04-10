#ifndef STARTRACKER_CORE_LOGGER_H
#define STARTRACKER_CORE_LOGGER_H

#include <StarEngine/DateTime.hpp>

#include <vector>
#include <mutex>
#include <format>

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
		template <typename ...Args>
		[[nodiscard]] static bool Info(Args&& ...args) noexcept {

			if (info.Mutex.try_lock()) {

				std::string entry = std::format(std::forward<Args>(args)...);
				std::string infoEntry = std::format("[INFO] {}", entry);
				std::string format = formatEntry(infoEntry);
				std::fprintf(stdout, "%s\n", format.c_str());
				
				info.List.emplace_back(format);
				info.Mutex.unlock();

				return true;
			}
			else {

				return false;
			}
		}

		template <typename ...Args>
		[[nodiscard]] static bool Warn(Args&& ...args) noexcept {

			if (warn.Mutex.try_lock()) {

				std::string entry = std::format(std::forward<Args>(args)...);
				std::string warnEntry = std::format("[WARN] {}", entry);
				std::string format = formatEntry(warnEntry);
				std::fprintf(stderr, "%s\n", format.c_str());

				warn.List.emplace_back(format);
				warn.Mutex.unlock();

				return true;
			}
			else {

				return false;
			}
		}

		template <typename ...Args>
		[[nodiscard]] static bool Error(Args&& ...args) noexcept {

			if (error.Mutex.try_lock()) {

				std::string entry = std::format(std::forward<Args>(args)...);
				std::string errorEntry = std::format("[ERROR] {}", entry);
				std::string format = formatEntry(errorEntry);
				std::fprintf(stderr, "%s\n", format.c_str());

				error.List.emplace_back(formatEntry(format));
				error.Mutex.unlock();

				return true;
			}
			else {

				return false;
			}
		}

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

#define STARTRACKER_INFO(...) (void)StarTracker::Core::Logger::Info(__VA_ARGS__) 
#define STARTRACKER_WARN(...) (void)StarTracker::Core::Logger::Warn(__VA_ARGS__) 
#define STARTRACKER_ERROR(...) (void)StarTracker::Core::Logger::Error(__VA_ARGS__) 

#endif // STARTRACKER_CORE_LOGGER_H