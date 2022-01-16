#ifndef STARTRACKER_CORE_UIVIEW_H
#define STARTRACKER_CORE_UIVIEW_H

#include "View.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace StarTracker::Core {

	// Workaround for enum class, so we don't have to write out the cast when using
	// ImGui->GetIO().Fonts->Fonts[UIFont::Regular];

	struct UIFont {

		inline static constexpr int Light = 0;
		inline static constexpr int Regular = 1;
		inline static constexpr int Medium = 2;
		inline static constexpr int Semibold = 3;
		inline static constexpr int Bold = 4;
		inline static constexpr int Heavy = 5;
		inline static constexpr int Italic = 6;
	};

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

