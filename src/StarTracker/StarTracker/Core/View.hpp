#ifndef STARTRACKER_CORE_VIEW_H
#define STARTRACKER_CORE_VIEW_H

#include <cstdint>

namespace StarTracker::Core {

    class View {

    protected:
        void* nativeWindowHandle;

    public:
        explicit View(void* nativeWindowHandle) noexcept;

        /**
        * @brief Sets the nativeWindowHandle, which is a GLFWwindow* internally
        *
        * @param nativeWindowHandle type-punned GLFWwindow*
        *
        */
        void SetNativeWindowHandle(void* nativeWindowHandle) noexcept;

        /**
        * @brief Gets called once on startup by the owning application instance
        *
        */
        virtual void OnInit() noexcept = 0;

        /**
        * @brief Gets called once on shutdown by the owning application instance
        *
        */
        virtual void OnDestroy() noexcept = 0;

        /**
        * @brief Gets called every frame by the owning application instance
        *
        */
        virtual void OnUpdate(float deltaTime) noexcept = 0;
    };
}

#endif // STARTRACKER_CORE_VIEW_H

