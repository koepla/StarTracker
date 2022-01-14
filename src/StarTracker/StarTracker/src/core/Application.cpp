#include "Application.hpp"

namespace StarTracker::Core {

	Application::Application(const ApplicationData& applicationData) noexcept : 
		window{ 
			WindowData { 
				.Width = applicationData.Width,
				.Height = applicationData.Height,
				.Title = applicationData.Title,
				.Running = true,
				.Fullscreen = applicationData.Fullscreen
			} 
		}, 
		userInterfaceView{ nullptr, applicationData.EnableDockspace } 
	{
		viewList = std::vector<View*>{};
		userInterfaceView.SetNativeWindowHandle(window.GetNativeHandle());
		instance = this;
	}

	void Application::Run() noexcept {

		userInterfaceView.OnInit();
		for (auto* view : viewList) {

			view->OnInit();
		}

		auto lastFrameTime = glfwGetTime();

		while (window.IsRunning()) {

			glClear(GL_COLOR_BUFFER_BIT);

			const auto time = glfwGetTime();
			const auto deltaTime = time - lastFrameTime;
			lastFrameTime = time;

			userInterfaceView.UIBegin();
			for (auto* view : viewList) {

				view->OnUpdate(deltaTime);
			}
			userInterfaceView.UIEnd();

			window.Update();
		}

		for (auto* view : viewList) {

			view->OnDestroy();
		}
		userInterfaceView.OnDestroy();
	}

	void Application::Close() noexcept {

		window.GetWindowData().Running = false;
	}

	void Application::AddToViewList(View* view) noexcept {

		viewList.emplace_back(view);
	}

	Window& Application::GetWindow() noexcept {

		return window;
	}

	inline Application* Application::GetInstance() noexcept {

		return instance;
	}
}