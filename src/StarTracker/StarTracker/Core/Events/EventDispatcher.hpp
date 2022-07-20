#ifndef STARTRACKER_CORE_EVENTS_EVENTDISPATCHER_H
#define STARTRACKER_CORE_EVENTS_EVENTDISPATCHER_H

#include <StarTracker/Core/Events/Event.hpp>

#include <vector>

namespace StarTracker::Core::Events {

    class EventDispatcher {

    private:
        std::vector<EventHandler> eventHandlerList;

    public:
        EventDispatcher() noexcept;

        void DispatchEvent(const Event& event) noexcept;
        void RegisterEventHandler(const EventHandler& eventHandler) noexcept;
    };
}

#endif // STARTRACKER_CORE_EVENTS_EVENTDISPATCHER_H