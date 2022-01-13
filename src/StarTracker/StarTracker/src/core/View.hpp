#ifndef STARTRACKER_CORE_VIEW_H
#define STARTRACKER_CORE_VIEW_H

#include <cstdint>

namespace StarTracker::Core {

	class View {

	private:
		void* nativeWindowHandle;

	public:
		explicit View(void* nativeWindowHandle) noexcept;
		
		virtual void OnInit() noexcept = 0;
		virtual void OnDestroy() noexcept = 0;
		virtual void OnUpdate(float deltaTime) noexcept = 0;
	};
}

#endif // STARTRACKER_CORE_VIEW_H

