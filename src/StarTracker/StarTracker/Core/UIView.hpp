#ifndef STARTRACKER_CORE_UIVIEW_H
#define STARTRACKER_CORE_UIVIEW_H

#include <StarTracker/Core/Core.hpp>
#include <StarTracker/Core/View.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace StarTracker::Core {

	struct UIFont {

		inline static ImFont* Light = nullptr;
		inline static ImFont* Regular = nullptr;
		inline static ImFont* Medium = nullptr;
		inline static ImFont* SemiBold = nullptr;
		inline static ImFont* Bold = nullptr;
		inline static ImFont* Heavy = nullptr;
		inline static ImFont* Italic = nullptr;
	};

	class UIView final : public View {

	private:
		bool enableDockSpace;

	public:
		explicit UIView(void* nativeWindowHandle, bool enableDockSpace = true) noexcept;

		void UIBegin() noexcept;
		void UIEnd() noexcept;
		virtual void OnInit() noexcept override;
		virtual void OnUpdate(float deltaTime) noexcept override;
		virtual void OnDestroy() noexcept override;
	};
}


#endif // STARTRACKER_CORE_UIVIEW_H

