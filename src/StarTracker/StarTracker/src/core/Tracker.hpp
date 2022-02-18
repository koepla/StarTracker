#ifndef STARTRACKER_CORE_TRACKER_H
#define STARTRACKER_CORE_TRACKER_H

#include <StarAPI/StarAPI.hpp>
#include "../utils/Stopwatch.hpp"
#include "../utils/serial/Package.hpp"
#include "../utils/serial/Serial.hpp"

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
		[[nodiscard]] bool Track(const std::shared_ptr<Ephemeris::CelestialBody>& object, const Ephemeris::Coordinates::Observer& observer, std::size_t duration, TrackerCallback callback) noexcept(false);
	
	private:
		[[nodiscard]] bool sendPackage(Utils::Serial::Pack32 package, bool enableMaxWait = false, std::size_t maxWait = 500) noexcept(false);
	};
}

#endif // STARTRACKER_CORE_TRACKER_H