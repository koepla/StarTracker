#include "View.hpp"

namespace StarTracker::Core {

	View::View(void* windowHandle) noexcept : nativeWindowHandle{ windowHandle } {

	}

	void View::SetNativeWindowHandle(void* windowHandle) noexcept {

		nativeWindowHandle = windowHandle;
	}
}