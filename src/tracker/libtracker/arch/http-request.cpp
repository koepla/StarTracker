#include "http-request.hpp"

namespace arch {

    HttpRequestException::HttpRequestException(std::string_view message) noexcept : message{ message } { }

    const char* HttpRequestException::what() const noexcept {
        return message.data();
    }
}// namespace arch
