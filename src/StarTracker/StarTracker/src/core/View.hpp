#ifndef STARTRACKER_CORE_VIEW_H
#define STARTRACKER_CORE_VIEW_H

#include <cstdint>

namespace StarTracker::Core {

	class View {

	protected:
		void* nativeWindowHandle;

	public:
		explicit View(void* nativeWindowHandle) noexcept;

		void SetNativeWindowHandle(void* nativeWindowHandle) noexcept;
		
		virtual void OnInit() noexcept = 0;
		virtual void OnDestroy() noexcept = 0;
		virtual void OnUpdate(float deltaTime) noexcept = 0;
	};
}

#endif // STARTRACKER_CORE_VIEW_H

