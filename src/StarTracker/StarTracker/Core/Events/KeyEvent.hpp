#ifndef STARTRACKER_CORE_EVENTS_KEYEVENT_H
#define STARTRACKER_CORE_EVENTS_KEYEVENT_H

#include <StarTracker/Core/InputCode.hpp>
#include <StarTracker/Core/Events/Event.hpp>

namespace StarTracker::Core::Events {

    enum class KeyStatus : std::uint16_t {

        None = 0,
        Pressed,
        Repeated,
        Released
    };

    class KeyEvent : public Event {

    private:
        KeyCode keyCode;
        KeyStatus keyStatus;

    public:
        KeyEvent(KeyCode keyCode, KeyStatus keyStatus) noexcept;

        [[nodiscard]] KeyCode GetKeyCode() const noexcept;
        [[nodiscard]] bool IsPressed() const noexcept;
        [[nodiscard]] bool IsRepeated() const noexcept;
        [[nodiscard]] bool IsReleased() const noexcept;
        [[nodiscard]] virtual std::string_view GetName() const noexcept override;
    };
}

#endif // STARTRACKER_CORE_EVENTS_KEYEVENT_H

