#include "package.hpp"

const char* CommandToString(Command command) noexcept {
    switch (command) {
        case Command::None:
            return "None";
        case Command::Wakeup:
            return "Wakeup";
        case Command::Sleep:
            return "Sleep";
        case Command::Move:
            return "Move";
        case Command::Configure:
            return "Configure";
        case Command::Origin:
            return "Origin";
        case Command::Ack:
            return "Acknowledge";
        case Command::Advance:
            return "Advance";
    }
    return "";
}
