#ifndef STARTRACKER_CORE_UIVIEW_H
#define STARTRACKER_CORE_UIVIEW_H

#include <StarTracker/Core/Core.hpp>
#include <StarTracker/Core/View.hpp>
#include <StarTracker/UI/UI.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace StarTracker::Core {

	class UIView : public View {

	private:
		bool enableDockSpace;

	public:
		explicit UIView(void* nativeWindowHandle, bool enableDockSpace = true) noexcept;

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