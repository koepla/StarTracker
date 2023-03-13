#ifndef LIBTRACKER_TRACKING_H
#define LIBTRACKER_TRACKING_H

/**
 * @author Elias Engelbert Plank
 * @date 2022-2023
 */

#include "core/view.hpp"

/**
 * The tracking view provides functionality for selecting celestial bodies, tracking them, analyzing them and
 * comparing them. It also serves as a quick access to frequently changed settings like tracking duration and
 * angular speed
 */
class Tracking : public View {
public:
    /**
     * Creates a new tracking view, does pretty much nothing as subsystem initialization is done by workspace
     * @param windowHandle Handle to the native window
     */
    explicit Tracking(void* windowHandle) noexcept;

    /**
     * Default destructor, does nothing
     */
    ~Tracking() noexcept override = default;

    /**
     * Initialization is handled by the constructor
     */
    void OnInit() noexcept override;

    /**
     * Renders the entire tracking view, consisting of location information, tracker status, quick settings and a
     * comprehensive list of celestial bodies, that can be filtered to ones desire
     * @param deltaTime Time since the last frame
     */
    void OnUpdate(f32 deltaTime) noexcept override;

    /**
     * Disconnects the tracker
     */
    void OnDestroy() noexcept override;

private:
    /**
     * Stores an index of the ImGuiListClipper, that handles iterating through the quite large catalog of celestial
     * bodies. As its name suggests, it stores the starting index or iterator
     */
    s32 bodyRenderStartIndex;

    /**
     * Stores an index of the ImGuiListClipper, that handles iterating through the quite large catalog of celestial
     * bodies. As its name suggests, it stores the end index or iterator
     */
    s32 bodyRenderEndIndex;
};

/**
 * Internal namespace houses functions that would be in an anonymous namespace, but are exposed as they are required by
 * another translation unit
 */
namespace Internal {

    /**
     * Draws a header that displays the current date, time, geographic location and tracker status. Internally, this
     * calls `DrawTrackerInfoCard` and `DrawTrackingDurationCard`
     */
    void DrawInfoHeader() noexcept;

    /**
     * Draws information regarding the status of the tracking mount, as well as the geographic location of the user, and
     * the current local time
     * @param width Width of the card
     * @param height Height of the card
     */
    void DrawTrackerInfoCard(f32 width, f32 height) noexcept;

    /**
     * Draws a card which houses a drag input field for controlling the tracking duration
     * @param width Width of the card
     * @param height Height of the card
     */
    void DrawTrackingDurationCard(f32 width, f32 height) noexcept;

    /**
     * Draws a card which houses a drag input field for controlling the angular speed
     * @param width Width of the card
     * @param height Height of the card
     */
    void DrawAngularSpeedCard(f32 width, f32 height) noexcept;
}// namespace Internal

#endif// LIBTRACKER_TRACKING_H
