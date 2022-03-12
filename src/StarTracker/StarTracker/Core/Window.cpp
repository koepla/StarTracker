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
        glfwSetMouseButtonCallback(nativeHandle, [](GLFWwindow* handle, int button, int action, int mods) -> void {

            const auto windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(handle));

            const auto mouseClickEvent = [&]() -> Events::MouseClickEvent {

                switch(action) {

                    case GLFW_PRESS: {

                        return Events::MouseClickEvent{ static_cast<MouseCode>(button), Events::MouseStatus::Pressed };
                    }
                    case GLFW_RELEASE: {

                        return Events::MouseClickEvent{ static_cast<MouseCode>(button), Events::MouseStatus::Released };
                    }
                    default: {

                        return Events::MouseClickEvent{ static_cast<MouseCode>(button), Events::MouseStatus::None };
                    }
                }
            }();

            windowData->EventDispatcher.DispatchEvent(mouseClickEvent);
        });
        glfwSetScrollCallback(nativeHandle, [](GLFWwindow* handle, double dx, double dy) -> void {

            const auto windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(handle));

            Events::MouseScrollEvent mouseScrollEvent{ dx, dy };
            windowData->EventDispatcher.DispatchEvent(mouseScrollEvent);
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

        const auto errorCallback = [](GLenum source,
                                      GLenum type,
                                      GLuint id,
                                      GLenum severity,
                                      GLsizei length,
                                      const GLchar* message,
                                      const void* userParam )-> void {

            if(severity == GL_DEBUG_SEVERITY_NOTIFICATION) {

                return;
            }

            const auto severityToString = [](GLenum severity) -> const char* {

                switch(severity) {

                    case GL_DEBUG_SEVERITY_HIGH: {

                        return "HIGH_SEVERITY";
                    }
                    case GL_DEBUG_SEVERITY_MEDIUM: {

                        return "MEDIUM_SEVERITY";
                    }
                    case GL_DEBUG_SEVERITY_LOW: {

                        return "LOW_SEVERITY";
                    }
                    case GL_DEBUG_SEVERITY_NOTIFICATION: {

                        return "NOTIFICATION_SEVERITY";
                    }
                    default: {

                        return "UNKNOWN_SEVERITY";
                    }
                }
            };

            std::fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = %s, message = %s\n",
                          ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
                          type, severityToString(severity), message );
        };

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(errorCallback, nullptr);
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

