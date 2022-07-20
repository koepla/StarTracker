#ifndef STARTRACKER_CORE_EVENTS_MOUSESCROLLEVENT_H
#define STARTRACKER_CORE_EVENTS_MOUSESCROLLEVENT_H

#include <StarTracker/Core/Events/Event.hpp>

namespace StarTracker::Core::Events {

    class MouseScrollEvent : public Event {

    private:
        double dx;
        double dy;

    public:
        MouseScrollEvent(double dx, double dy) noexcept;

        [[nodiscard]] double GetDeltaX() const noexcept;
        [[nodiscard]] double GetDeltaY() const noexcept;

        [[nodiscard]] virtual std::string_view GetName() const noexcept override;
    };
}

#endif // STARTRACKER_CORE_EVENTS_MOUSESCROLLEVENT_H