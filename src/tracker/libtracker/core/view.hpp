#ifndef LIBTRACKER_CORE_VIEW_H
#define LIBTRACKER_CORE_VIEW_H

#include "utility/types.hpp"

class View {
public:
    explicit View(void* nativeWindowHandle) noexcept;
    virtual ~View() noexcept = default;

    /**
     * @brief Sets the nativeWindowHandle, which is a GLFWwindow* internally
     * @param nativeWindowHandle type-punned GLFWwindow*
     */
    void SetNativeWindowHandle(void* nativeWindowHandle) noexcept;

    /**
     * @brief Gets called once on startup by the owning application instance
     */
    virtual void OnInit() noexcept = 0;

    /**
     * @brief Gets called once on shutdown by the owning application instance
     */
    virtual void OnDestroy() noexcept = 0;

    /**
     * @brief Gets called every frame by the owning application instance
     */
    virtual void OnUpdate(f32 deltaTime) noexcept = 0;

protected:
    void* nativeWindowHandle;
};

class UIView : public View {
public:
    explicit UIView(void* windowHandle, bool enableDockSpace = true) noexcept;

    /**
     * @brief Called every frame before the viewList of the owning application instance is updated
     *        Is responsible for rendering the DockSpace
     * @see Application
     */
    void UIBegin() const noexcept;

    /**
     * @brief Called every frame after the viewList of the owning application instance is updated
     * @see Application
     */
    void UIEnd() const noexcept;

    /**
     * @brief Called once on startup before the viewList of the owning application instance is initialized
     *        Creates the ImGui context
     * @see Application
     */
    void OnInit() noexcept override;

    /**
     * @brief Does nothing
     */
    void OnUpdate(f32 deltaTime) noexcept override;

    /**
     * @brief Called once on destroy after the viewList of the owning application instance is destroyed
     *        Destroys the ImGui context
     *
     * @see Application
     */
    void OnDestroy() noexcept override;

private:
    bool enableDockSpace;
};

#endif// LIBTRACKER_CORE_VIEW_H
