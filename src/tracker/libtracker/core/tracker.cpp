#include <optional>

#include "core.hpp"
#include "input.hpp"
#include "location-manager.hpp"
#include "settings.hpp"
#include "tracker.hpp"

TrackerHandle::TrackerHandle() noexcept : status{ TrackerStatus::Idle }, begin{ DateTime::Now() } { }

bool TrackerHandle::InProgress() const noexcept {
    // We don't want to lock the handle here, so we copy its value so that it can't change
    // within the boolean or
    const auto localStatus = status;
    return localStatus == TrackerStatus::Tracking || localStatus == TrackerStatus::Slewing;
}

DateTime TrackerHandle::GetBegin() const noexcept {
    return InProgress() ? begin : DateTime::Now();
}

TrackerStatus TrackerHandle::GetStatus() const noexcept {
    return status;
}

s64 TrackerHandle::GetElapsedSeconds() const noexcept {
    return DateTime::Difference(GetBegin(), DateTime::Now());
}

void TrackerHandle::SetStatus(TrackerStatus trackerStatus) noexcept {
    std::unique_lock lock(mutex);
    status = trackerStatus;
}

bool TrackerHandle::IsAbortable() const noexcept {
    return status == TrackerStatus::Tracking;
}

bool TrackerHandle::Abort() noexcept {
    if (!IsAbortable()) {
        return false;
    }
    std::unique_lock lock(mutex);
    status = TrackerStatus::Aborted;
    return true;
}

void Tracker::Initialize() noexcept {
    serialPort = arch::SerialPort::Create();
}

bool Tracker::Connect() noexcept {
    std::unique_lock lock(mutex);
    try {
        if (serialPort->IsOpen()) {
            serialPort->Close();
        }
        serialPort->Open(Settings::Get<std::string>("Tracker-Port"), 115200);
        return serialPort->IsOpen();
    } catch (...) {
        return false;
    }
}

bool Tracker::Disconnect() noexcept {
    std::unique_lock lock(mutex);
    try {
        if (!serialPort->IsOpen()) {
            return true;
        }
        serialPort->Close();
        return !serialPort->IsOpen();
    } catch (...) {
        return false;
    }
}

bool Tracker::IsConnected() noexcept {
    try {
        static bool connectionStatus = false;
        if (mutex.try_lock()) {
            connectionStatus = serialPort->IsOpen();
            mutex.unlock();
        }
        return connectionStatus;
    } catch (...) {
        return false;
    }
}

std::vector<std::string> Tracker::GetPortNames() noexcept {
    return arch::SerialPort::GetPortNames();
}

ephemeris::Horizontal Tracker::GetPosition() noexcept {
    return { Settings::Get<f64>("Tracker-Altitude"), Settings::Get<f64>("Tracker-Azimuth") };
}

bool Tracker::Submit(const ephemeris::Horizontal& position) noexcept {
    if (Handle && Handle->InProgress()) {
        return false;
    }
    Handle = std::make_shared<TrackerHandle>();
    utility::FireAndForget([position]() -> void {
        std::unique_lock lock(mutex);
        const Pack32 acknowledgePackage{ Command::Ack };
        if (!sendPackage(acknowledgePackage, true)) {
            Handle->SetStatus(TrackerStatus::Failure);
            return;
        }

        Pack32 trackingPackage{ Command::Move };
        trackingPackage.Push(static_cast<f32>(position.Altitude));
        trackingPackage.Push(static_cast<f32>(position.Azimuth));
        trackingPackage.Push(static_cast<f32>(Settings::Get<f64>("Tracker-AngularSpeed", 0.5)));

        Handle->SetStatus(TrackerStatus::Slewing);
        if (!sendPackage(trackingPackage)) {
            Handle->SetStatus(TrackerStatus::Failure);
        } else {
            Handle->SetStatus(TrackerStatus::Idle);
        }
    });
    return true;
}

bool Tracker::SubmitPlanet(const std::shared_ptr<ephemeris::Planet>& planet, f64 duration) noexcept {
    if (Handle && Handle->InProgress()) {
        return false;
    }
    Handle = std::make_shared<TrackerHandle>();
    utility::FireAndForget([planet, duration]() -> void {
        std::unique_lock lock(mutex);
        const Pack32 acknowledgePackage{ Command::Ack };
        if (!sendPackage(acknowledgePackage, true)) {
            Handle->SetStatus(TrackerStatus::Failure);
            return;
        }

        Handle->SetStatus(TrackerStatus::Slewing);
        Stopwatch durationWatch{};
        durationWatch.Start();

        while (durationWatch.GetElapsedMilliseconds() / 1000.0 < duration) {
            const auto currentPosition = ObserveGeographic(planet->GetEquatorialPosition(DateTime::Now()),
                                                           LocationManager::GetGeographic(), DateTime::Now());
            Pack32 trackingPackage{ Command::Move };
            trackingPackage.Push(static_cast<f32>(currentPosition.Altitude));
            trackingPackage.Push(static_cast<f32>(currentPosition.Azimuth));
            trackingPackage.Push(static_cast<f32>(Settings::Get<f64>("Tracker-AngularSpeed")));

            if (!sendPackage(trackingPackage)) {
                LIBTRACKER_ERROR("Could not send package or received invalid response. Abort tracking.");
                Handle->SetStatus(TrackerStatus::Failure);
                return;
            }

            switch (Handle->GetStatus()) {
                case TrackerStatus::Aborted:
                    return;
                case TrackerStatus::Slewing:
                    Handle->SetStatus(TrackerStatus::Tracking);
                    break;
                case TrackerStatus::Idle:
                case TrackerStatus::Tracking:
                case TrackerStatus::Failure:
                    break;
            }
        }
        Handle->SetStatus(TrackerStatus::Idle);
    });
    return true;
}

bool Tracker::SubmitFixed(const std::shared_ptr<ephemeris::FixedBody>& body, f64 duration) noexcept {
    Handle = std::make_shared<TrackerHandle>();
    utility::FireAndForget([body, duration]() -> void {
        std::unique_lock lock(mutex);
        const Pack32 acknowledgePackage{ Command::Ack };
        if (!sendPackage(acknowledgePackage, true)) {
            Handle->SetStatus(TrackerStatus::Failure);
            return;
        }

        Handle->SetStatus(TrackerStatus::Slewing);
        Stopwatch durationWatch{};
        durationWatch.Start();

        while (durationWatch.GetElapsedMilliseconds() / 1000.0 < duration) {
            const auto now = DateTime::Now();
            const auto currentPosition =
                    ObserveGeographic(body->GetEquatorialPosition(now), LocationManager::GetGeographic(), now);
            Pack32 trackingPackage{ Command::Move };
            trackingPackage.Push(static_cast<f32>(currentPosition.Altitude));
            trackingPackage.Push(static_cast<f32>(currentPosition.Azimuth));
            trackingPackage.Push(static_cast<f32>(Settings::Get<f64>("Tracker-AngularSpeed")));

            if (!sendPackage(trackingPackage)) {
                LIBTRACKER_ERROR("Could not send package or received invalid response. Abort tracking.");
                Handle->SetStatus(TrackerStatus::Failure);
                return;
            }
            switch (Handle->GetStatus()) {
                case TrackerStatus::Aborted:
                    return;
                case TrackerStatus::Slewing:
                    Handle->SetStatus(TrackerStatus::Tracking);
                    break;
                case TrackerStatus::Idle:
                case TrackerStatus::Tracking:
                case TrackerStatus::Failure:
                    break;
            }
        }
        Handle->SetStatus(TrackerStatus::Idle);
    });
    return true;
}

bool Tracker::SubmitSteering(const SteeringMemory& memory) noexcept {
    // We may not start steering when there is an ongoing tracking process
    if (Handle && Handle->InProgress()) {
        return false;
    }

    // We need to check if the steering memory is valid
    if (!(memory.Left && memory.Right && memory.Up && memory.Down)) {
        return false;
    }

    Handle = std::make_shared<TrackerHandle>();
    utility::FireAndForget([memory] {
        Pack32 acknowledgePackage{ Command::Ack };
        if (!sendPackage(acknowledgePackage, true)) {
            LIBTRACKER_ERROR("Could not steer the tracker, handshake failed");
            Handle->SetStatus(TrackerStatus::Failure);
            return;
        } else {
            Handle->SetStatus(TrackerStatus::Tracking);
        }

        while (Handle->InProgress()) {
            auto left = Input::IsKeyPressed(KeyCode::Left) || *memory.Left;
            left = left || Input::IsJoystickPressed(JoystickDirection::Left);

            auto right = Input::IsKeyPressed(KeyCode::Right) || *memory.Right;
            right = right || Input::IsJoystickPressed(JoystickDirection::Right);

            auto up = Input::IsKeyPressed(KeyCode::Up) || *memory.Up;
            up = up || Input::IsJoystickPressed(JoystickDirection::Up);

            auto down = Input::IsKeyPressed(KeyCode::Down) || *memory.Down;
            down = down || Input::IsJoystickPressed(JoystickDirection::Down);

            const auto angularSpeed = Settings::Get<f64>("Tracker-AngularSpeed");
            const auto angle = static_cast<f32>(angularSpeed) / 4.0f;

            Input::IsJoystickPressed(JoystickDirection::Up);

            Pack32 steeringPackage{ Command::Advance };

            f32 pitchDifference = 0.0f;
            if (up && !down) {
                pitchDifference = angle;
            } else if (down && !up) {
                pitchDifference = -angle;
            }

            f32 yawDifference = 0.0f;
            if (left && !right) {
                yawDifference = -angle;
            } else if (right && !left) {
                yawDifference = angle;
            }

            if (pitchDifference != 0.0f || yawDifference != 0.0f) {
                steeringPackage.Push(pitchDifference);
                steeringPackage.Push(yawDifference);
                steeringPackage.Push(static_cast<f32>(angularSpeed));
                if (!sendPackage(steeringPackage)) {
                    LIBTRACKER_ERROR("Could not steer the tracker, upload failed");
                    Handle->SetStatus(TrackerStatus::Failure);
                    return;
                }
            }
        }
    });
    return true;
}

bool Tracker::UpdateConfig() noexcept {
    if (Handle && Handle->InProgress()) {
        return false;
    }

    std::unique_lock lock(mutex);
    const Pack32 acknowledgePackage{ Command::Ack };
    if (!sendPackage(acknowledgePackage, true)) {
        LIBTRACKER_ERROR("Could not update the config, handshake with tracker failed");
        return false;
    }

    const auto altitude = Settings::Get<f64>("Tracker-Altitude");
    const auto azimuth = Settings::Get<f64>("Tracker-Azimuth");
    const auto rmsCurrent = Settings::Get<f64>("Tracker-RmsCurrent");
    const auto gearRatio = Settings::Get<f64>("Tracker-GearRatio");
    const auto microSteps = Settings::Get<usize>("Tracker-MicroSteps");

    Pack32 configurePackage{ Command::Configure };
    configurePackage.Push(static_cast<f32>(altitude));
    configurePackage.Push(static_cast<f32>(azimuth));
    configurePackage.Push(static_cast<f32>(rmsCurrent));
    configurePackage.Push(static_cast<f32>(gearRatio));
    configurePackage.Push(static_cast<f32>(microSteps));

    if (!sendPackage(configurePackage)) {
        LIBTRACKER_ERROR("Could not update the config, upload to tracker failed");
        return false;
    }
    return true;
}

bool Tracker::sendPackage(Pack32 package, bool failAfterTimeout) noexcept {
    try {
        Pack32 responsePackage{};
        if (!serialPort->IsOpen()) {
            return false;
        }

        if (Settings::Get<bool>("Output-Verbose")) {
            LIBTRACKER_INFO("Sending package => ({})", CommandToString(package.GetFlag()));
        }

        auto sendBuffer = reinterpret_cast<uint8_t*>(&package);
        const auto sentBytes = serialPort->Write(sendBuffer, sizeof package);
        PackageHistory.emplace_back(package, PackageDirection::Outgoing);

        Stopwatch stopwatch{};
        stopwatch.Start();

        while (serialPort->Available() != sizeof responsePackage) {
            if (failAfterTimeout) {
                if (stopwatch.GetElapsedMilliseconds() > 750.0) {
                    return false;
                }
            }
        }

        auto responseBuffer = reinterpret_cast<uint8_t*>(&responsePackage);
        const auto receivedBytes = serialPort->Read(responseBuffer, sizeof responsePackage);
        const auto responseFlag = responsePackage.GetFlag();

        PackageHistory.emplace_back(responsePackage, PackageDirection::Ingoing);
        const auto historyLimit = Settings::Get<usize>("Tracker-PackageHistoryLimit", 200);
        if (PackageHistory.size() > historyLimit) {
            PackageHistory = { PackageHistory.end() -
                                       static_cast<decltype(PackageHistory)::difference_type>(historyLimit),
                               PackageHistory.end() };
        }

        if (Settings::Get<bool>("Output-Verbose")) {
            LIBTRACKER_INFO("Received package => ({})", CommandToString(responseFlag));
        }

        Settings::Get<f64>("Tracker-Altitude") = static_cast<f64>(responsePackage.Read<f32>(0));
        Settings::Get<f64>("Tracker-Azimuth") = static_cast<f64>(responsePackage.Read<f32>(1));

        const auto sentBytesMatch = sentBytes == sizeof package;
        const auto receivedBytesMatch = receivedBytes == sizeof package;
        const auto responseFlagMatch = responseFlag == Command::Ack;
        return sentBytesMatch && receivedBytesMatch && responseFlagMatch;
    } catch (const arch::SerialException& e) {
        LIBTRACKER_ERROR("SerialException caught: {}", e.what());
        return false;
    }
}

const char* TrackerStatusToString(TrackerStatus status) noexcept {
    switch (status) {
        case TrackerStatus::Idle:
            return "Idle";
        case TrackerStatus::Slewing:
            return "Slewing";
        case TrackerStatus::Tracking:
            return "Tracking";
        case TrackerStatus::Failure:
            return "Failure";
        case TrackerStatus::Aborted:
            return "Aborted";
    }
    return "";
}

const char* PackageDirectionToString(Tracker::PackageDirection direction) noexcept {
    switch (direction) {
        case Tracker::PackageDirection::Ingoing:
            return "In";
        case Tracker::PackageDirection::Outgoing:
            return "Out";
    }
    return "";
}

Tracker::PackageHistoryEntry::PackageHistoryEntry(const Pack32& package, PackageDirection direction) noexcept
    : Package(package),
      Direction(direction),
      Time(DateTime::Now()) { }
