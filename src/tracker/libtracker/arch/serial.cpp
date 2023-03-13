#include "serial.hpp"

namespace arch {

    SerialException::SerialException(std::string_view message) noexcept : message{ message } { }

    const char* SerialException::what() const noexcept {
        return message.data();
    }
}// namespace arch
