#ifndef STARTRACKER_CORE_WINDOW_H
#define STARTRACKER_CORE_WINDOW_H

#include "Assert.hpp"
#include "Core.hpp"

#include "events/Event.hpp"
#include "events/KeyEvent.hpp"
#include "events/MouseMoveEvent.hpp"
#include "events/WindowResizeEvent.hpp"
#include "events/EventDispatcher.hpp"

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

		void Update() noexcept;

		[[nodiscard]] WindowData& GetWindowData() noexcept;
		[[nodiscard]] std::int32_t GetWidth() const noexcept;
		[[nodiscard]] std::int32_t GetHeight() const noexcept;
		[[nodiscard]] std::string_view GetTitle() const noexcept;
		[[nodiscard]] GLFWwindow* GetNativeHandle() const noexcept;
		[[nodiscard]] bool IsRunning() const noexcept;
	};
}

#endif // STARTRACKER_CORE_WINDOW_H
