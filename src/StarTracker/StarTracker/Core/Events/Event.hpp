#ifndef STARTRACKER_CORE_EVENTS_EVENT_H
#define STARTRACKER_CORE_EVENTS_EVENT_H

#include <functional>
#include <string_view>

namespace StarTracker::Core::Events {

    class Event {

    public:
        [[nodiscard]] virtual std::string_view GetName() const noexcept = 0;
    };

    using EventHandler = std::function<void(const Event&)>;
}

#endif // STARTRACKER_CORE_EVENTS_EVENT_H