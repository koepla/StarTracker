#include "Window.hpp"

namespace StarTracker::Core {

    Window::Window(const WindowData& windowData) noexcept : windowData{ windowData }, nativeHandle{ nullptr } {

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        nativeHandle = glfwCreateWindow(
            windowData.Width,
            windowData.Height,
            windowData.Title.c_str(),
            windowData.Fullscreen ? glfwGetPrimaryMonitor() : nullptr,
            nullptr
        );

        if (!nativeHandle) {

            ASSERT(false && "Couldn't create window.");
            glfwDestroyWindow(nativeHandle);
            glfwTerminate();
        }

        glfwMakeContextCurrent(nativeHandle);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {

            ASSERT(false && "Couldn't load OpenGL functions.");
            glfwDestroyWindow(nativeHandle);
            glfwTerminate();
        }

        glfwSwapInterval(1);
        glfwSetWindowUserPointer(nativeHandle, &this->windowData);
        glfwSetWindowSizeCallback(nativeHandle, [](GLFWwindow* handle, int width, int height) -> void {

            const auto windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(handle));
            windowData->Width = width;
            windowData->Height = height;
            glViewport(0, 0, width, height);
        });
        glfwSetWindowCloseCallback(nativeHandle, [](GLFWwindow* handle) -> void {

            const auto windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(handle));
            windowData->Running = false;
        });

        this->windowData.Running = true;
    }

    Window::~Window() noexcept {

        glfwDestroyWindow(nativeHandle);
        glfwTerminate();
    }

    void Window::Update() noexcept {

        glfwPollEvents();
        glfwSwapBuffers(nativeHandle);
    }

    WindowData& Window::GetWindowData() noexcept {

        return windowData;
    }

    std::int32_t Window::GetWidth() const noexcept {

        return windowData.Width;
    }

    std::int32_t Window::GetHeight() const noexcept {

        return windowData.Height;
    }

    std::string_view Window::GetTitle() const noexcept {

        return std::string_view{ windowData.Title };
    }

    GLFWwindow* Window::GetNativeHandle() const noexcept {

        return nativeHandle;
    }

    bool Window::IsRunning() const noexcept {

        return windowData.Running;
    }
}

