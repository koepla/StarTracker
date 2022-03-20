#ifndef STARTRACKER_CORE_TRACKER_H
#define STARTRACKER_CORE_TRACKER_H

#include <StarEngine/StarEngine.hpp>
#include <StarTracker/Utils/Stopwatch.hpp>
#include <StarTracker/Utils/Serial/Package.hpp>
#include <StarTracker/Utils/Serial/Serial.hpp>

#include <thread>

namespace StarTracker::Core {

	enum class TrackerCallbackStatus {

		FAILURE = 0,
		SUCCESS = 1
	};

	using TrackerCallback = std::function<void(TrackerCallbackStatus)>;

	class Tracker {

    private:
		std::atomic<bool> tracking;
		Utils::Serial::SerialPort serialPort;

	public:
		Tracker() noexcept = default;

		[[nodiscard]] bool Connect() noexcept(false);
		[[nodiscard]] bool Disconnect() noexcept(false);
		[[nodiscard]] bool IsConnected() noexcept(false);
		[[nodiscard]] bool Track(const std::shared_ptr<Ephemeris::CelestialBody>& object, const Ephemeris::Coordinates::Observer& observer, double duration, const TrackerCallback& callback) noexcept(false);
	
	private:
		[[nodiscard]] bool sendPackage(Utils::Serial::Pack32 package, bool enableMaxWait = false, double maxWait = 500.0) noexcept(false);
	};
}

#endif // STARTRACKER_CORE_TRACKER_H