#ifndef STARTRACKER_CORE_APPLICATION_H
#define STARTRACKER_CORE_APPLICATION_H

#include <StarTracker/Core/View.hpp>
#include <StarTracker/Core/UIView.hpp>
#include <StarTracker/Core/Window.hpp>

#include <vector>

namespace StarTracker::Core {

    struct ApplicationData {

        std::int32_t Width;
        std::int32_t Height;
        std::string Title;
        bool EnableDockSpace;
        bool Fullscreen;
        bool VerticalSync;
    };

    class Application {

    protected:
        static inline Application* instance = nullptr;

    protected:
        Window window;
        UIView userInterfaceView;
        std::vector<View*> viewList;

    public:
        Application() = delete;
        Application(const Application&) = delete;
        explicit Application(const ApplicationData& applicationData) noexcept;

        /**
        * @brief Runs the application
        *
        */
        void Run() noexcept;

        /**
        * @brief Closes the application
        *
        */
        void Close() noexcept;

        /**
        * @brief Registers the view to the internal viewList
        *
        * @param view pointer to the view
        *
        */
        void RegisterView(View* view) noexcept;

        /**
        * @brief Registers the eventHandler to the window-owned eventDispatcher
        *
        * @param eventHandler eventHandler function
        *
        */
        void RegisterEventHandler(const Events::EventHandler& eventHandler) noexcept;

        /**
        * @brief Returns a reference to the window
        *
        * @return window instance
        *
        */
        [[nodiscard]] Window& GetWindow() noexcept;

    public:
        /**
        * @brief Returns a pointer to the singleton application instance
        *
        * @return application instance
        *
        */
        [[nodiscard]] static Application* GetInstance() noexcept;
    };
}

#endif // STARTRACKER_CORE_APPLICATION_H