#include "View.hpp"

namespace StarTracker::Core {

	View::View(void* nativeWindowHandle) noexcept : nativeWindowHandle{ nativeWindowHandle } {

	}

	void View::SetNativeWindowHandle(void* nativeWindowHandle) noexcept {

		this->nativeWindowHandle = nativeWindowHandle;
	}
}

