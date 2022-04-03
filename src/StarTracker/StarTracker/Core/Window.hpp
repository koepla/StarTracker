#ifndef STARTRACKER_CORE_WINDOW_H
#define STARTRACKER_CORE_WINDOW_H

#include <StarTracker/Core/Assert.hpp>
#include <StarTracker/Core/Core.hpp>

#include <StarTracker/Core/Events/Event.hpp>
#include <StarTracker/Core/Events/KeyEvent.hpp>
#include <StarTracker/Core/Events/MouseClickEvent.hpp>
#include <StarTracker/Core/Events/MouseMoveEvent.hpp>
#include <StarTracker/Core/Events/MouseScrollEvent.hpp>
#include <StarTracker/Core/Events/WindowResizeEvent.hpp>
#include <StarTracker/Core/Events/EventDispatcher.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

namespace StarTracker::Core {

	struct WindowData {

		std::int32_t Width;
		std::int32_t Height;
		std::string Title;
		bool Running;
		bool Fullscreen;
		bool VerticalSync;

		Events::EventDispatcher EventDispatcher;
	};

	class Window {

	private:
		GLFWwindow* nativeHandle;
		WindowData windowData;

	public:
		Window() = delete;
		Window(const Window&) = delete;
		explicit Window(const WindowData& windowData) noexcept;
		~Window() noexcept;

		/**
		* @brief Polls events and swaps front and back buffer
		*
		*/
		void Update() noexcept;

		/**
		* @brief Hides the mouse cursor
		*
		*/
		void HideCursor() noexcept;

		/**
		* @brief Shows the mouse cursor
		*
		*/
		void ShowCursor() noexcept;

		/**
		* @brief Returns a reference to the windowData member
		*        WindowData contains the size, title and state of the window
		*
		* @return reference to windowData member
		* 
		*/
		[[nodiscard]] WindowData& GetWindowData() noexcept;

		/**
		* @brief Returns the width of the window
		*
		* @return width of the window (obtained from windowData)
		* 
		*/
		[[nodiscard]] std::int32_t GetWidth() const noexcept;

		/**
		* @brief Returns the height of the window
		*
		* @return height of the window (obtained from windowData)
		* 
		*/
		[[nodiscard]] std::int32_t GetHeight() const noexcept;

		/**
		* @brief Returns the title of the window
		*
		* @return title of the window (obtained from windowData)
		* 
		*/
		[[nodiscard]] std::string_view GetTitle() const noexcept;

		/**
		* @brief Returns the GLFW specific window handle
		*
		* @return GLFW-handle of the window
		* 
		*/
		[[nodiscard]] GLFWwindow* GetNativeHandle() const noexcept;

		/**
		* @brief Returns if the window is running or wants to close
		*
		* @return true if the window is still running, false if the window wants to close
		* 
		*/
		[[nodiscard]] bool IsRunning() const noexcept;
	};
}

#endif // STARTRACKER_CORE_WINDOW_H
