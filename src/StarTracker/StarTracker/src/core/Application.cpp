#include "Application.hpp"

namespace StarTracker::Core {

	Application::Application(const ApplicationData& applicationData) noexcept : window{ applicationData } {

		instance = this;
	}

	void Application::Run() noexcept {

		float lastFrameTime = 0.0f;

		while (window.IsRunning()) {

			float time = glfwGetTime();
			float deltaTime = time - lastFrameTime;
			lastFrameTime = time;

			window.Update();
		}
	}

	void Application::Close() noexcept {

		window.GetWindowData().Running = false;
	}

	Window& Application::GetWindow() noexcept {

		return window;
	}

	inline Application* Application::GetInstance() noexcept {

		return instance;
	}
}