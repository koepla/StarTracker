#ifndef LIBTRACKER_CONTROL_H
#define LIBTRACKER_CONTROL_H

/**
 * @author Elias Engelbert Plank
 * @date 2022-2023
 */

#include <libtracker/libtracker.hpp>

/**
 * The control view provides functionality for configuring the tracking mount and its parameters. Additionally, it
 * houses a console that displays log messages
 */
class Control : public View {
public:
    /**
     * Creates a new Control view instance
     * @param windowHandle
     */
    explicit Control(void* windowHandle) noexcept;

    /**
     * Only implemented for the sake of polymorphism
     */
    void OnInit() noexcept override;

    /**
     * Only implemented for the sake of polymorphism
     */
    void OnDestroy() noexcept override;

    /**
     * Draws controls for configuring the tracking mount, and a console that displays past and current log messages
     * @param deltaTime Time since the last frame
     */
    void OnUpdate(float deltaTime) noexcept override;
};


#endif// LIBTRACKER_CONTROL_H
