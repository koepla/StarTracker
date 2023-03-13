#include "events.hpp"

/**
 * @brief This is not assigned to default, to avoid emitting v-table to every translation unit
 */
Event::~Event() noexcept { }// NOLINT(modernize-use-equals-default)

void EventDispatcher::DispatchEvent(const Event& event) const noexcept {
    for (auto& handler : eventHandlerList) {
        handler(event);
    }
}

void EventDispatcher::RegisterEventHandler(const EventHandler& eventHandler) noexcept {
    eventHandlerList.emplace_back(eventHandler);
}

KeyEvent::KeyEvent(KeyCode keyCode, KeyStatus keyStatus) noexcept : keyCode{ keyCode }, keyStatus{ keyStatus } { }

KeyCode KeyEvent::GetKeyCode() const noexcept {
    return keyCode;
}

bool KeyEvent::IsPressed() const noexcept {
    return keyStatus == KeyStatus::Pressed || keyStatus == KeyStatus::Repeated;
}

bool KeyEvent::IsRepeated() const noexcept {
    return keyStatus == KeyStatus::Repeated;
}

bool KeyEvent::IsReleased() const noexcept {
    return keyStatus == KeyStatus::Released;
}

std::string_view KeyEvent::GetName() const noexcept {
    return std::string_view{ "KeyEvent" };
}

MouseClickEvent::MouseClickEvent(MouseCode mouseCode, MouseStatus mouseStatus) noexcept
    : mouseCode{ mouseCode },
      mouseStatus{ mouseStatus } { }

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

MouseMoveEvent::MouseMoveEvent(f64 x, f64 y) noexcept : x{ x }, y{ y } { }

f64 MouseMoveEvent::GetX() const noexcept {
    return x;
}

f64 MouseMoveEvent::GetY() const noexcept {
    return y;
}

std::string_view MouseMoveEvent::GetName() const noexcept {
    return std::string_view{ "MouseMoveEvent" };
}

MouseScrollEvent::MouseScrollEvent(f64 dx, f64 dy) noexcept : dx{ dx }, dy{ dy } { }

f64 MouseScrollEvent::GetDeltaX() const noexcept {
    return dx;
}

f64 MouseScrollEvent::GetDeltaY() const noexcept {
    return dy;
}

std::string_view MouseScrollEvent::GetName() const noexcept {
    return std::string_view{ "MouseScrollEvent" };
}

WindowResizeEvent::WindowResizeEvent(s32 width, s32 height) noexcept : width{ width }, height{ height } { }

s32 WindowResizeEvent::GetWidth() const noexcept {
    return width;
}

s32 WindowResizeEvent::GetHeight() const noexcept {
    return height;
}

std::string_view WindowResizeEvent::GetName() const noexcept {
    return std::string_view{ "WindowResizeEvent" };
}
