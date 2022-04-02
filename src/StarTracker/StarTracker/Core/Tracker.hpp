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

		/**
		* @brief Tries to connect to the tracker by iterating through all serialports
		*        If a valid serialport is found, the method connects to it
		*
		* @return true if a serialport was found, false if not
		*
		* @throws SerialException if one of the SerialPort methods fails
		* 
		*/
		[[nodiscard]] bool Connect() noexcept(false);

		/**
		* @brief Tries to disconnect to the tracker
		*
		* @return true if the disconnect was successful
		*
		* @throws SerialException if one of the SerialPort methods has failed
		* 
		*/
		[[nodiscard]] bool Disconnect() noexcept(false);

		/**
		* @brief Checks if the tracker is still connected or not
		*
		* @return true if the tracker is still connected
		*
		* @throws SerialException if one of the SerialPort methods has failed
		* 
		*/
		[[nodiscard]] bool IsConnected() noexcept(false);

		/**
		* @brief Tracks the specified CelestialBody instance
		*
		* @param object std::shared_ptr to the CelestialBody target instance
		*
		* @param observer location of the observer
		*
		* @param duration the duration for the tracking process
		*
		* @param callback callback which gets called on certain events (finished, failed)
		*
		* @return true if the tracking was successful
		*
		* @throws SerialException if one of the SerialPort methods has failed
		* 
		*/
		[[nodiscard]] bool Track(const std::shared_ptr<Ephemeris::CelestialBody>& object, const Ephemeris::Coordinates::Observer& observer, double duration, const TrackerCallback& callback) noexcept(false);
	
	private:
		/**
		* @brief sends a package to the tracker, checks for ack-flag
		*
		* @param package package that shall be sent to the tracker
		*
		* @param enableMaxWait wait a certain amount of time for the response
		*
		* @param maxWait if enableMaxWait is set to true, maxWait milliseconds are waited before we decide that the sending has failed
		*
		* @return true if the package was sent successfully
		*
		* @throws SerialException if one of the SerialPort methods has failed
		* 
		*/
		[[nodiscard]] bool sendPackage(Utils::Serial::Pack32 package, bool enableMaxWait = false, double maxWait = 500.0) noexcept(false);
	};
}

#endif // STARTRACKER_CORE_TRACKER_H