#ifndef STARTRACKER_CORE_UIVIEW_H
#define STARTRACKER_CORE_UIVIEW_H

#include "View.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace StarTracker::Core {

	class UIView : public View {

	private:
		bool enableDockspace;

	public:
		explicit UIView(void* nativeWindowHandle, bool enableDockspace = true) noexcept;

		void UIBegin() noexcept;
		void UIEnd() noexcept;
		virtual void OnInit() noexcept override;
		virtual void OnUpdate(float deltaTime) noexcept override;
		virtual void OnDestroy() noexcept override;
	};
}


#endif // STARTRACKER_CORE_UIVIEW_H

