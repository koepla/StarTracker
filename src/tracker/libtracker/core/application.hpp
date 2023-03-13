#ifndef LIBTRACKER_CORE_APPLICATION_H
#define LIBTRACKER_CORE_APPLICATION_H

#include <memory>
#include <vector>

#include "utility/types.hpp"
#include "view.hpp"
#include "window.hpp"

struct ApplicationData {
    s32 Width;
    s32 Height;
    std::string Title;
    bool EnableDockSpace;
    bool Fullscreen;
    bool VerticalSync;
};

class Application {
public:
    Application() = delete;
    Application(const Application&) = delete;
    explicit Application(const ApplicationData& applicationData) noexcept;

    /**
     * @brief Runs the application
     */
    void Run() noexcept;

    /**
     * @brief Closes the application
     */
    void Close() noexcept;

    /**
     * @brief Registers the view to the internal viewList
     * @param view pointer to the view
     */
    void RegisterView(std::shared_ptr<View> view) noexcept;

    /**
     * @brief Registers the eventHandler to the window-owned eventDispatcher
     * @param eventHandler eventHandler function
     */
    void RegisterEventHandler(const EventHandler& eventHandler) noexcept;

    /**
     * @brief Returns a reference to the window
     * @return window instance
     */
    Window& GetWindow() noexcept;

    /**
     * @brief Returns a pointer to the singleton application instance
     * @return application instance
     */
    static Application* GetInstance() noexcept;

protected:
    static inline Application* instance = nullptr;
    Window window;
    UIView userInterfaceView;
    std::vector<std::shared_ptr<View>> viewList;

private:
    f64 lastFrameTime;
    void iteration() noexcept;
};

#endif// LIBTRACKER_CORE_APPLICATION_H