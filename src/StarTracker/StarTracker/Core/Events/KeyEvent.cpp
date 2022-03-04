#include "KeyEvent.hpp"

namespace StarTracker::Core::Events {

	KeyEvent::KeyEvent(KeyCode keyCode, KeyStatus keyStatus) noexcept : keyCode{ keyCode }, keyStatus{ keyStatus } {
	
	}

	KeyCode KeyEvent::GetKeyCode() const noexcept {

		return keyCode;
	}

	bool KeyEvent::IsPressed() const noexcept {

		return keyStatus == KeyStatus::Pressed || keyStatus == KeyStatus::Repeated;
	}

	bool KeyEvent::IsRepeated() const noexcept {

		return keyStatus == KeyStatus::Repeated;
	}

	bool KeyEvent::IsReleased() const noexcept {

		return keyStatus == KeyStatus::Released;
	}

	std::string_view KeyEvent::GetName() const noexcept {

		return std::string_view{ "KeyEvent" };
	}
}