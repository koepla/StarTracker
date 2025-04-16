#ifndef UTILITY_CONVERSION_H
#define UTILITY_CONVERSION_H

/**
 * Collection of conversion functions, especially string conversions
 * @author Elias Engelbert Plank
 * @date 2023/01/11
 */

#include <charconv>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>

namespace utility {

    /**
     * Tries to obtain a value of GenType from the specified view
     * @tparam GenType Generic type, however there must be an implementation for it in std::from_chars
     * @param view View where the value is in
     * @return optional value
     */
    template<typename GenType>
    std::enable_if_t<std::is_integral_v<GenType>, std::optional<GenType>> FromString(std::string_view view) noexcept {
        GenType value{};
        const auto conversion = std::from_chars(view.data(), view.data() + view.size(), value);
        if (conversion.ec == std::errc()) {
            return value;
        }
        return std::nullopt;
    }

    // For floating-point types (float, double, etc.)
    template<typename GenType>
    std::enable_if_t<std::is_floating_point_v<GenType>, std::optional<GenType>> FromString(
            std::string_view view) noexcept {
        GenType value{};
        std::stringstream ss{ std::string{ view } };// construct std::string for stream
        ss >> value;
        if (ss.fail()) {
            return std::nullopt;
        }
        return value;
    }

    /**
     * Splits an input of StringType into a target container
     * @tparam ContainerType Type of the target container
     * @tparam StringType Type of the input string
     * @param container Target container
     * @param input Input
     * @param delimiter Delimiter
     */
    template<typename ContainerType, typename StringType>
    void Split(ContainerType& container, StringType input, StringType delimiter) noexcept {
        std::size_t posStart = 0;
        std::size_t posEnd;
        const auto delimiterLength = delimiter.length();

        std::string_view token{};
        while ((posEnd = input.find(delimiter, posStart)) != std::string_view::npos) {
            token = input.substr(posStart, posEnd - posStart);
            posStart = posEnd + delimiterLength;
            if (!token.empty()) {
                container.emplace_back(token);
            }
        }

        const auto last = input.substr(posStart);
        if (!last.empty()) {
            container.emplace_back(last);
        }
    }
}// namespace utility

#endif// UTILITY_CONVERSION_H
