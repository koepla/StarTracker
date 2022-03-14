#include "Tracker.hpp"

namespace StarTracker::Core {

	bool Tracker::Connect() noexcept(false) {

		if (serialPort.IsOpen()) {

			return false;
		}

		const auto availablePorts = Utils::Serial::SerialPort::GetPortNames();

		if (availablePorts.size() == 0) {

			return false;
		}

		for (const auto& currentPort : availablePorts) {

			serialPort.Open(currentPort, 115200);

			if (!serialPort.IsOpen()) {

				continue;
			}

			if (sendPackage(Utils::Serial::Pack32{ Utils::Serial::Command::ACK }, true, 500)) {

				return true;
			}
			else {

				serialPort.Close();
				continue;
			}
		}

		return false;
	}

	bool Tracker::Disconnect() noexcept(false) {

		if (!serialPort.IsOpen()) {

			return true;
		}

		serialPort.Close();

		return !serialPort.IsOpen();
	}

	bool Tracker::IsConnected() noexcept(false) {

		return serialPort.IsOpen();
	}

	bool Tracker::Track(const std::shared_ptr<Ephemeris::CelestialBody>& object, const Ephemeris::Coordinates::Observer& observer, double duration, TrackerCallback callback) noexcept(false) {

		if (tracking.load()) {

			return false;
		}

		auto trackingJob = std::thread{ [this, object, observer, duration, callback]() -> void {

			Utils::Serial::Pack32 acknowledgePackage{ Utils::Serial::Command::ACK };
			if (!sendPackage(acknowledgePackage, true)) {

				callback(TrackerCallbackStatus::FAILURE);
				return;
			}

			tracking.store(true);

			Utils::Diagnostics::Stopwatch durationWatch{};
			durationWatch.Start();

			while (durationWatch.GetEllapsedMilliseconds() < duration) {

				const auto currentPosition = Ephemeris::Coordinates::Transform::TerrestrialObserverToHorizontal(
					object->GetSphericalPosition(DateTime::Now()),
					observer,
					DateTime::Now()
				);

				Utils::Serial::Pack32 trackingPackage{ Utils::Serial::Command::MOVE };
				trackingPackage.Push<float>(static_cast<float>(currentPosition.Altitude));
				trackingPackage.Push<float>(static_cast<float>(currentPosition.Azimuth));

				if (!sendPackage(trackingPackage)) {

					tracking.store(false);
					callback(TrackerCallbackStatus::FAILURE);
					return;
				}
			}

			tracking.store(false);
			callback(TrackerCallbackStatus::SUCCESS);
		} };
		trackingJob.detach();

		return true;
	}

	bool Tracker::sendPackage(Utils::Serial::Pack32 package, bool enableMaxWait, std::size_t maxWait) noexcept(false) {

		Utils::Serial::Pack32 responsePackage{};

		if (!serialPort.IsOpen()) {

			return false;
		}

		Utils::Diagnostics::Stopwatch stopwatch{};
		stopwatch.Start();

		const auto sentBytes = serialPort.Write(reinterpret_cast<uint8_t*>(&package), sizeof package);

		while (serialPort.Available() != sizeof responsePackage) {

			if (enableMaxWait) {

				if (stopwatch.GetEllapsedMilliseconds() > maxWait) {

					return false;
				}
			}
		}

		const auto receivedBytes = serialPort.Read(reinterpret_cast<uint8_t*>(&responsePackage), sizeof responsePackage);
		const auto responseFlag = responsePackage.GetFlag();

		return
			sentBytes == sizeof package &&
			receivedBytes == sizeof responsePackage &&
			responseFlag == Utils::Serial::Command::ACK;
	}
}
