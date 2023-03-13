#include "window.hpp"
#include "asset-database.hpp"
#include "core.hpp"

Window::Window(const WindowData& windowData) noexcept : nativeHandle{ nullptr }, windowData{ windowData } {
    LIBTRACKER_INFO("Initializing Window");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    nativeHandle = glfwCreateWindow(windowData.Width, windowData.Height, windowData.Title.c_str(),
                                    windowData.Fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    if (!nativeHandle) {
        LIBTRACKER_ASSERT(false, "Couldn't create window");
        glfwDestroyWindow(nativeHandle);
        glfwTerminate();
    }

    glfwMakeContextCurrent(nativeHandle);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        LIBTRACKER_ASSERT(false, "Couldn't load OpenGL functions");
        glfwDestroyWindow(nativeHandle);
        glfwTerminate();
    }

    glfwSwapInterval(windowData.VerticalSync ? 1 : 0);
    glfwSetWindowUserPointer(nativeHandle, &this->windowData);
    glfwSetFramebufferSizeCallback(nativeHandle, [](GLFWwindow* handle, int width, int height) -> void {
        /*
         * In this case the window is minimized, we don't want to react to it,
         * because it could mess with our code (it does in fact mess with our code)
         */
        if (width == 0 && height == 0) {
            return;
        }

        const auto windowData = static_cast<WindowData*>(glfwGetWindowUserPointer(handle));
        windowData->Width = width;
        windowData->Height = height;
        glViewport(0, 0, width, height);

        const WindowResizeEvent windowResizeEvent{ width, height };
        windowData->Dispatcher.DispatchEvent(windowResizeEvent);
    });
    glfwSetCursorPosCallback(nativeHandle, [](GLFWwindow* handle, f64 x, f64 y) -> void {
        const auto windowData = static_cast<WindowData*>(glfwGetWindowUserPointer(handle));
        const MouseMoveEvent mouseMoveEvent{ x, y };
        windowData->Dispatcher.DispatchEvent(mouseMoveEvent);
    });
    glfwSetMouseButtonCallback(nativeHandle, [](GLFWwindow* handle, int button, int action, int mods) -> void {
        const auto windowData = static_cast<WindowData*>(glfwGetWindowUserPointer(handle));
        const auto mouseClickEvent = [&]() -> MouseClickEvent {
            switch (action) {
                case GLFW_PRESS: {
                    return MouseClickEvent{ static_cast<MouseCode>(button), MouseStatus::Pressed };
                }
                case GLFW_RELEASE: {
                    return MouseClickEvent{ static_cast<MouseCode>(button), MouseStatus::Released };
                }
                default: {
                    return MouseClickEvent{ static_cast<MouseCode>(button), MouseStatus::None };
                }
            }
        }();

        windowData->Dispatcher.DispatchEvent(mouseClickEvent);
    });
    glfwSetScrollCallback(nativeHandle, [](GLFWwindow* handle, f64 dx, f64 dy) -> void {
        const auto windowData = static_cast<WindowData*>(glfwGetWindowUserPointer(handle));
        const MouseScrollEvent mouseScrollEvent{ dx, dy };
        windowData->Dispatcher.DispatchEvent(mouseScrollEvent);
    });
    glfwSetKeyCallback(nativeHandle, [](GLFWwindow* handle, int key, int, int action, int) -> void {
        const auto windowData = static_cast<WindowData*>(glfwGetWindowUserPointer(handle));
        const auto keyEvent = [&]() -> KeyEvent {
            switch (action) {
                case GLFW_PRESS: {
                    return KeyEvent{ static_cast<KeyCode>(key), KeyStatus::Pressed };
                }
                case GLFW_REPEAT: {
                    return KeyEvent{ static_cast<KeyCode>(key), KeyStatus::Repeated };
                }
                case GLFW_RELEASE: {
                    return KeyEvent{ static_cast<KeyCode>(key), KeyStatus::Released };
                }
                default: {
                    return KeyEvent{ static_cast<KeyCode>(key), KeyStatus::None };
                }
            }
        }();

        windowData->Dispatcher.DispatchEvent(keyEvent);
    });
    glfwSetWindowCloseCallback(nativeHandle, [](GLFWwindow* handle) -> void {
        const auto windowData = static_cast<WindowData*>(glfwGetWindowUserPointer(handle));
        windowData->Running = false;
    });

    this->windowData.Running = true;
    const auto errorCallback = [](GLenum, GLenum type, GLuint, GLenum severity, GLsizei, const GLchar* message,
                                  const void*) -> void {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
            return;
        }

        const auto severityToString = [](GLenum severity) -> const char* {
            switch (severity) {
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

        if (type == GL_DEBUG_TYPE_ERROR) {
            LIBTRACKER_ERROR("[OpenGL]: type = {}, severity = {}, message = {}", type, severityToString(severity),
                             message);
        } else {
            LIBTRACKER_WARN("[OpenGL]: type = {}, severity = {}, message = {}", type, severityToString(severity),
                            message);
        }
    };

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(errorCallback, nullptr);

    auto icon = AssetDatabase::LoadIcon("256x256.png");
    glfwSetWindowIcon(nativeHandle, 1, &icon);
    free(icon.pixels);
}

Window::~Window() noexcept {
    glfwDestroyWindow(nativeHandle);
    glfwTerminate();
}

void Window::Update() const noexcept {
    glfwPollEvents();
    glfwSwapBuffers(nativeHandle);
}

WindowData& Window::GetWindowData() noexcept {
    return windowData;
}

s32 Window::GetWidth() const noexcept {
    return windowData.Width;
}

s32 Window::GetHeight() const noexcept {
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
