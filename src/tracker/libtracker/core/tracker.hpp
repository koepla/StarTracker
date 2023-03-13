#ifndef LIBTRACKER_CORE_TRACKER_H
#define LIBTRACKER_CORE_TRACKER_H

#include <atomic>
#include <thread>

#include <libengine/libengine.hpp>

#include "arch/serial.hpp"
#include "package.hpp"
#include "stopwatch.hpp"
#include "utility/async.hpp"
#include "utility/types.hpp"

/**
 * Enum for representing the status of the tracking mount
 */
enum class TrackerStatus : u16 { Idle, Slewing, Tracking, Failure, Aborted };

/**
 * TrackerHandle is a class for cross thread communication in a tracking process
 */
class TrackerHandle {
public:
    TrackerHandle() noexcept;

    /**
     * Checks if the tracking is still in progress
     * @return true If the tracking is still in progress
     * @return false If not
     */
    bool InProgress() const noexcept;

    /**
     * Returns the begin of the tracking
     * @return Begin
     */
    DateTime GetBegin() const noexcept;

    /**
     * Checks the status of the tracking
     * @return Status
     */
    TrackerStatus GetStatus() const noexcept;

    /**
     * Returns the number of seconds that have passed since tracking started
     * @return Seconds
     */
    s64 GetElapsedSeconds() const noexcept;

    /**
     * Sets the status of the tracker
     * @param trackerStatus The status
     */
    void SetStatus(TrackerStatus trackerStatus) noexcept;

    /**
     * Indicates whether the tracking process can be aborted or not
     * @return bool
     */
    bool IsAbortable() const noexcept;

    /**
     * Aborts the tracking process
     * @return Indicates if the abortion of the tracking processing failed
     */
    bool Abort() noexcept;

private:
    std::mutex mutex;
    TrackerStatus status;
    DateTime begin;
};

/**
 * Main tracking mount interface
 */
class Tracker {
public:
    /**
     * Initializes the tracker, this creates the SerialPort internally
     */
    static void Initialize() noexcept;

    /**
     * Tries to connect to the tracker
     * @return true if a serial port was found, false if not
     */
    static bool Connect() noexcept;

    /**
     * Tries to disconnect from the tracker
     * @return true if the disconnect was successful
     */
    static bool Disconnect() noexcept;

    /**
     * Checks if the tracker is still connected or not
     * @return true if the tracker is still connected
     */
    static bool IsConnected() noexcept;

    /**
     * Retrieves the names of the available serial ports
     * @return list of ports
     */
    static std::vector<std::string> GetPortNames() noexcept;

    /**
     * Returns the latest position of the tracker
     * @return position, as stored in the settings properties
     */
    static ephemeris::Horizontal GetPosition() noexcept;

    /**
     * Moves the tracker to the specified position
     * @param position AltAz position
     * @return bool that indicates if the job could be started
     */
    static bool Submit(const ephemeris::Horizontal& position) noexcept;

    /**
     * Tracks the specified Planet
     * @param planet std::shared_ptr to the Planet target instance
     * @param duration The duration for the tracking process in seconds
     * @return bool that indicates if the job could be started
     */
    static bool SubmitPlanet(const std::shared_ptr<ephemeris::Planet>& planet, f64 duration) noexcept;

    /**
     * Tracks the specified FixedBody
     * @param body std::shared_ptr to the FixedBody target instance
     * @param duration The duration for the tracking process in seconds
     * @return bool that indicates if the job could be started
     */
    static bool SubmitFixed(const std::shared_ptr<ephemeris::FixedBody>& body, f64 duration) noexcept;

    /**
     * Memory addresses of boolean values that indicate the direction when manually steering the tracking mount
     */
    struct SteeringMemory {
        bool* Left;
        bool* Right;
        bool* Up;
        bool* Down;
    };

    /**
     * Starts a listener that responds to keyboard input and the values that were passed as the steering memory. This
     * acts as a sort of free mode where the tracker moves based of the users input
     * @param memory Memory handle to the values that are checked for steering
     * @return bool that indicates if the steering could be started
     */
    static bool SubmitSteering(const SteeringMemory& memory) noexcept;

    /**
     * Updates the tracker configuration. Internally, this reads the settings properties for the position, current,
     * micro steps and gear ratio, and then uploads it to the tracking mount
     * @return bool that indicates if the update was successful
     */
    static bool UpdateConfig() noexcept;

public:
    /**
     * Global tracking handle that manages cross-thread communication
     */
    static inline std::shared_ptr<TrackerHandle> Handle;

    /**
     * Indicates whether the package was outbound or inbound
     */
    enum class PackageDirection : u16 { Ingoing, Outgoing };

    /**
     * Entry for storing meta information of a package
     */
    struct PackageHistoryEntry {
        Pack32 Package;
        PackageDirection Direction;
        DateTime Time;

        /**
         * Creates a new history entry, the Time is set to now
         * @param package Package
         * @param direction Direction of the package
         */
        explicit PackageHistoryEntry(const Pack32& package, PackageDirection direction) noexcept;
    };

    /**
     * History of packages
     */
    static inline std::vector<PackageHistoryEntry> PackageHistory;

private:
    /**
     * @brief Sends a package to the tracker, checks for ack-flag
     * @param package Package that shall be sent to the tracker
     * @param failAfterTimeout Wait a certain amount of time for the response, if the time is up sending has failed
     * @return true if the package was sent successfully
     */
    static bool sendPackage(Pack32 package, bool failAfterTimeout = false) noexcept;

    /**
     * Used to guarantee thread safety, as each submission is executed asynchronously on a different thread
     */
    static inline std::mutex mutex;

    /**
     * Platform specific serial port handle that serves as a gateway to the tracking mount
     */
    static inline std::unique_ptr<arch::SerialPort> serialPort;
};

/**
 * Returns the string representation of each individual tracker status
 * @param status Status
 * @return string representation of status
 */
const char* TrackerStatusToString(TrackerStatus status) noexcept;

/**
 * Returns the string representation of a package direction
 * @param direction Direction
 * @return string representation of direction
 */
const char* PackageDirectionToString(Tracker::PackageDirection direction) noexcept;

#endif// LIBTRACKER_CORE_TRACKER_H