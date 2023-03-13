#include <fmt/color.h>

#include "core.hpp"

namespace {

    std::string formatEntry(std::string_view entry) noexcept {
        const auto now = DateTime::Now();
        return fmt::format("[{}] {}", now.ToString(), entry);
    }
}// namespace

void Logger::Info(const std::string& message) noexcept {
    std::unique_lock lock(Mutex);
    auto format = formatEntry(message);
    fmt::print(stdout, fg(fmt::color::lime_green), "[INFO] {}\n", format);

    auto& entry = List.emplace_back();
    entry.Message = std::move(format);
    entry.Sev = Severity::Info;
}

void Logger::Warn(const std::string& message) noexcept {
    std::unique_lock lock(Mutex);
    auto format = formatEntry(message);
    fmt::print(stderr, fg(fmt::color::orange), "[WARNING] {}\n", format);

    auto& entry = List.emplace_back();
    entry.Message = std::move(format);
    entry.Sev = Severity::Warn;
}

void Logger::Error(const std::string& message) noexcept {
    std::unique_lock lock(Mutex);
    auto format = formatEntry(message);
    fmt::print(stderr, fg(fmt::color::red), "[ERROR] {}\n", format);

    auto& entry = List.emplace_back();
    entry.Message = std::move(format);
    entry.Sev = Severity::Error;
}
const std::vector<Logger::Entry>& Logger::GetLog() noexcept {
    return List;
}