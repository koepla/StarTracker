#ifndef STARTRACKER_CORE_EVENTS_MOUSECLICKEVENT_H
#define STARTRACKER_CORE_EVENTS_MOUSECLICKEVENT_H

#include <StarTracker/Core/InputCode.hpp>
#include <StarTracker/Core/Events/Event.hpp>

namespace StarTracker::Core::Events {

    enum class MouseStatus : std::uint16_t {

        None = 0,
        Pressed,
        Released
    };

    class MouseClickEvent : public Event {

    private:
        MouseCode mouseCode;
        MouseStatus mouseStatus;

    public:
        MouseClickEvent(MouseCode mouseCode, MouseStatus mouseStatus) noexcept;

        [[nodiscard]] MouseCode GetMouseCode() const noexcept;
        [[nodiscard]] bool IsPressed() const noexcept;
        [[nodiscard]] bool IsReleased() const noexcept;
        [[nodiscard]] virtual std::string_view GetName() const noexcept override;
    };
}

#endif // STARTRACKER_CORE_EVENTS_MOUSECLICKEVENT_H


