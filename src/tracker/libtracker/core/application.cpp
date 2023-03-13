#include "application.hpp"

Application::Application(const ApplicationData& applicationData) noexcept
    : window{ WindowData{ applicationData.Width,
                          applicationData.Height,
                          applicationData.Title,
                          true,
                          applicationData.Fullscreen,
                          applicationData.VerticalSync,
                          {} } },
      userInterfaceView{ nullptr, applicationData.EnableDockSpace },
      lastFrameTime{ 0.0 } {
    userInterfaceView.SetNativeWindowHandle(window.GetNativeHandle());
    instance = this;
}

void Application::iteration() noexcept {
    glClear(GL_COLOR_BUFFER_BIT);
    const auto time = glfwGetTime();
    const auto deltaTime = time - lastFrameTime;
    lastFrameTime = time;

    userInterfaceView.UIBegin();
    for (const auto& view : viewList) {
        view->OnUpdate(static_cast<f32>(deltaTime));
    }
    userInterfaceView.UIEnd();
    window.Update();
}

void Application::Run() noexcept {
    userInterfaceView.OnInit();
    for (const auto& view : viewList) {
        view->OnInit();
    }

    RegisterEventHandler([this](const Event& event) {
        if (dynamic_cast<const WindowResizeEvent*>(&event)) {
            iteration();
        }
    });

    lastFrameTime = glfwGetTime();
    while (window.IsRunning()) {
        iteration();
    }

    for (const auto& view : viewList) {
        view->OnDestroy();
    }
    userInterfaceView.OnDestroy();
}

void Application::Close() noexcept {
    window.GetWindowData().Running = false;
}

void Application::RegisterView(std::shared_ptr<View> view) noexcept {
    viewList.emplace_back(view);
}

void Application::RegisterEventHandler(const EventHandler& eventHandler) noexcept {
    window.GetWindowData().Dispatcher.RegisterEventHandler(eventHandler);
}

Window& Application::GetWindow() noexcept {
    return window;
}

Application* Application::GetInstance() noexcept {
    return instance;
}
