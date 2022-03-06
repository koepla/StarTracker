#include "MouseClickEvent.hpp"

namespace StarTracker::Core::Events {

    MouseClickEvent::MouseClickEvent(MouseCode mouseCode, MouseStatus mouseStatus) noexcept : mouseCode{ mouseCode }, mouseStatus{ mouseStatus }{

    }

    MouseCode MouseClickEvent::GetMouseCode() const noexcept {

        return mouseCode;
    }

    bool MouseClickEvent::IsPressed() const noexcept {

        return mouseStatus == MouseStatus::Pressed;
    }

    bool MouseClickEvent::IsReleased() const noexcept {

        return mouseStatus == MouseStatus::Released;
    }

    std::string_view MouseClickEvent::GetName() const noexcept {

        return std::string_view{ "MouseClickEvent" };
    }
}


