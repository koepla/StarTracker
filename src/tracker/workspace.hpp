#ifndef LIBTRACKER_WORKSPACE_H
#define LIBTRACKER_WORKSPACE_H

/**
 * @author Elias Engelbert Plank
 * @date 2022-2023
 */

#include <control.hpp>
#include <processing.hpp>
#include "tracking.hpp"

/**
 * A workspace handles the registration of the other tabs, as well as the menu bar and the settings menu
 */
class Workspace : public View {
public:
    /**
     * Creates a new Workspace that handles registration of the other tabs, as well as the menu bar and the settings
     * menu. Every subsystem in libtracker is initialized here and before the registration of other tabs. There may only
     * be one workspace. There may be only one workspace
     * @param windowHandle Handle to the native window
     */
    explicit Workspace(void* windowHandle) noexcept;

    /**
     * Default destructor for workspace
     */
    ~Workspace() noexcept override = default;

    /**
     * Nothing is initialized here, as the heavy lifting is done in the constructor
     */
    void OnInit() noexcept override;

    /**
     * Draws the menu bar and its submenus, and checks for multi-key IO
     * @param deltaTime Time since the last frame
     */
    void OnUpdate(f32 deltaTime) noexcept override;

    /**
     * Unloads most libtracker subsystems, and serializes the settings file
     */
    void OnDestroy() noexcept override;

private:
    /**
     * Controls if the settings menu is open or closed
     */
    bool openSettings = false;

    /**
     * Instance of the tracking view
     * The tracking view provides functionality for selecting celestial bodies, tracking them, analyzing them and
     * comparing them. It also serves as a quick access to frequently changed settings like tracking duration and
     * angular speed
     */
    std::shared_ptr<Tracking> tracking;

    /**
     * Instance of the processing view
     * The processing view provides functionality for loading and processing images. It does this by utilizing little
     * programs, called `shaders` on the graphics card
     */
    std::shared_ptr<Processing> processing;

    /**
     * Instance of the control view
     * The control view provides functionality for configuring the tracking mount and its parameters. Additionally, it
     * houses a console that displays log messages
     */
    std::shared_ptr<Control> control;
};

#endif// LIBTRACKER_WORKSPACE_H