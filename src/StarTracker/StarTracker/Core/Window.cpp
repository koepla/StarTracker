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

        glfwSwapInterval(windowData.VerticalSync ? 1 : 0);
        glfwSetWindowUserPointer(nativeHandle, &this->windowData);
        glfwSetWindowSizeCallback(nativeHandle, [](GLFWwindow* handle, int width, int height) -> void {

            const auto windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(handle));
            windowData->Width = width;
            windowData->Height = height;
            glViewport(0, 0, width, height);

            Events::WindowResizeEvent windowResizeEvent{ width, height };
            windowData->EventDispatcher.DispatchEvent(windowResizeEvent);
        });
        glfwSetCursorPosCallback(nativeHandle, [](GLFWwindow* handle, double x, double y) -> void {

            const auto windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(handle));

            Events::MouseMoveEvent mouseMoveEvent{ x, y };
            windowData->EventDispatcher.DispatchEvent(mouseMoveEvent);
        });
        glfwSetKeyCallback(nativeHandle, [](GLFWwindow* handle, int key, int scancode, int action, int mods) -> void {

            const auto windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(handle));
            
            const auto keyEvent = [&]() -> Events::KeyEvent {

                switch (action) {

                    case GLFW_PRESS: {

                        return Events::KeyEvent{ static_cast<KeyCode>(key), Events::KeyStatus::Pressed };
                    }
                    case GLFW_REPEAT: {

                        return Events::KeyEvent{ static_cast<KeyCode>(key), Events::KeyStatus::Repeated };
                    }
                    case GLFW_RELEASE: {

                        return Events::KeyEvent{ static_cast<KeyCode>(key), Events::KeyStatus::Released };
                    }
                    default: {

                        return Events::KeyEvent{ static_cast<KeyCode>(key), Events::KeyStatus::None };
                    }
                }
            }();

            windowData->EventDispatcher.DispatchEvent(keyEvent);
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

