#include "WindowResizeEvent.hpp"

namespace StarTracker::Core::Events {

	WindowResizeEvent::WindowResizeEvent(std::int32_t width, std::int32_t height) noexcept : width{ width }, height{ height } {
	
	}

	std::int32_t WindowResizeEvent::GetWidth() const noexcept {

		return width; 
	}

	std::int32_t WindowResizeEvent::GetHeight() const noexcept {

		return height;
	}

	std::string_view WindowResizeEvent::GetName() const noexcept {

		return std::string_view{ "WindowResizeEvent" };
	}
}