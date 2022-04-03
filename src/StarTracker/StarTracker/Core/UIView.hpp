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

	enum class UITheme : std::int16_t {

		Light = 0,
		Dark = 1
	};

	class UIView : public View {

	private:
		bool enableDockSpace;

	public:
		explicit UIView(void* nativeWindowHandle, bool enableDockSpace = true) noexcept;

		/**
		* @brief Updates the theme
		*
		* @param theme UITheme enum
		*
		* @see UITheme
		* 
		*/
		static void SetTheme(UITheme theme) noexcept;

		/**
		* @brief Called every frame before the viewList of the owning application instance is updated
		*        Is responsible for rendering the DockSpace
		*
		* @see Application
		* 
		*/
		void UIBegin() const noexcept;

		/**
		* @brief Called every frame after the viewList of the owning application instance is updated
		*
		* @see Application
		* 
		*/
		void UIEnd() const noexcept;

		/**
		* @brief Called once on startup before the viewList of the owning application instance is initialized
		*        Creates the ImGui context 
		*
		* @see Application
		* 
		*/
		virtual void OnInit() noexcept override;

		/**
		* @brief Does nothing
		*		 
		*/
		virtual void OnUpdate(float deltaTime) noexcept override;

		/**
		* @brief Called once on destroy after the viewList of the owning application instance is destroyed
		*        Destroys the ImGui context
		*
		* @see Application
		* 
		*/
		virtual void OnDestroy() noexcept override;
	};
}


#endif // STARTRACKER_CORE_UIVIEW_H