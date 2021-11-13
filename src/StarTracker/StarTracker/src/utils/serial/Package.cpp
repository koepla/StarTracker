#include "Package.hpp"

namespace StarTracker::Utils::Serial {

	PackageException::PackageException(std::string&& message) : message(std::move(message)) {

	}

	const char* PackageException::what() const noexcept {

		return message.c_str();
	}

	Header::Header() : Flag(Command::NONE), Size(0) {

	}

	Header::Header(Command flag, uint8_t size) : Flag(flag), Size(size) {

	}
}