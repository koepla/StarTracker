#ifndef LIBTRACKER_CORE_WINDOW_H
#define LIBTRACKER_CORE_WINDOW_H

/**
 * @author Elias Engelbert Plank
 * @date 2022
 */

#include "arch/os.hpp"
#include "events.hpp"
#include "utility/types.hpp"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <string>

/**
 * Utility struct for storing window related data
 */
struct WindowData {
    s32 Width;
    s32 Height;
    std::string Title;
    bool Running;
    bool Fullscreen;
    bool VerticalSync;
    EventDispatcher Dispatcher;
};

/**
 * Window that manages polling of events, drawing and resizing. It is the foundation of the user interface
 */
class Window {
public:
    /**
     * Deleted as a window must have specifications
     */
    Window() = delete;

    /**
     * Deleted as a window should not be copied
     */
    Window(const Window&) = delete;

    /**
     * Create a new window instance with the corresponding graphics (OpenGL) context
     * @param windowData Window data
     */
    explicit Window(const WindowData& windowData) noexcept;

    /**
     * Terminates the graphics context and the window
     */
    ~Window() noexcept;

    /**
     * Polls events and swaps front and back buffer
     */
    void Update() const noexcept;

    /**
     * Returns a reference to the windowData member. WindowData contains the size, title and state of the window
     * @return reference to windowData member
     */
    WindowData& GetWindowData() noexcept;

    /**
     * Returns the width of the window
     * @return width of the window (obtained from windowData)
     */
    s32 GetWidth() const noexcept;

    /**
     * Returns the height of the window
     * @return height of the window (obtained from windowData)
     */
    s32 GetHeight() const noexcept;

    /**
     * Returns the title of the window
     * @return title of the window (obtained from windowData)
     */
    std::string_view GetTitle() const noexcept;

    /**
     * Returns the GLFW specific window handle
     * @return GLFW-handle of the window
     */
    GLFWwindow* GetNativeHandle() const noexcept;

    /**
     * Returns if the window is running or wants to close
     * @return true if the window is still running, false if the window wants to close
     */
    bool IsRunning() const noexcept;

private:
    /**
     * Native window handle that is managed by our windowing backbone, GLFW
     */
    GLFWwindow* nativeHandle;

    /**
     * Data related to the window
     */
    WindowData windowData;
};

#endif// LIBTRACKER_CORE_WINDOW_H
