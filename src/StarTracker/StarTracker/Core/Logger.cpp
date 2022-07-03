#include "Logger.hpp"

namespace StarTracker::Core {

    bool Logger::Info(std::string_view message) noexcept {

        if (info.Mutex.try_lock()) {

            std::string format = formatEntry(fmt::format("[INFO] {}", message));
            std::fprintf(stdout, "%s\n", format.c_str());

            info.List.emplace_back(format);
            info.Mutex.unlock();

            return true;
        }
        else {

            return false;
        }
    }

    bool Logger::Warn(std::string_view message) noexcept {

        if (warn.Mutex.try_lock()) {

            std::string format = formatEntry(fmt::format("[WARN] {}", message));
            std::fprintf(stdout, "%s\n", format.c_str());

            warn.List.emplace_back(format);
            warn.Mutex.unlock();

            return true;
        }
        else {

            return false;
        }
    }

    bool Logger::Error(std::string_view message) noexcept {

        if (error.Mutex.try_lock()) {

            std::string format = formatEntry(fmt::format("[ERROR] {}", message));
            std::fprintf(stdout, "%s\n", format.c_str());

            error.List.emplace_back(formatEntry(format));
            error.Mutex.unlock();

            return true;
        }
        else {

            return false;
        }
    }

	void Logger::ClearInfoList() noexcept {

		info.List.clear();
	}

	void Logger::ClearWarnList() noexcept {

		warn.List.clear();
	}

	void Logger::ClearErrorList() noexcept {

		error.List.clear();
	}

	std::vector<std::string> Logger::GetInfoList() noexcept {

		if (info.Mutex.try_lock()) {

			auto& list = info.List;
			info.Mutex.unlock();
			return list;
		}
		else {

			return {};
		}
	}

	std::vector<std::string> Logger::GetWarnList() noexcept {

		if (warn.Mutex.try_lock()) {

			auto& list = warn.List;
			warn.Mutex.unlock();
			return list;
		}
		else {

			return {};
		}
	}

	std::vector<std::string> Logger::GetErrorList() noexcept {

		if (error.Mutex.try_lock()) {

			auto& list = error.List;
			error.Mutex.unlock();
			return list;
		}
		else {

			return {};
		}
	}

	std::size_t Logger::GetInfoSize() noexcept {

		return info.List.size();
	}

	std::size_t Logger::GetWarnSize() noexcept {

		return warn.List.size();
	}

	std::size_t Logger::GetErrorSize() noexcept {

		return error.List.size();
	}

	std::string Logger::formatEntry(std::string_view entry) noexcept {
	
		const auto now = DateTime::Now();
		return fmt::format("[{}] {}", now.ToString(), entry);
	}
}