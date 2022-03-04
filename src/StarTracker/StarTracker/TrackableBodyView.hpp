#ifndef STARTRACKER_TRACKABLEBODYVIEW_H
#define STARTRACKER_TRACKABLEBODYVIEW_H

#include <StarEngine/StarEngine.hpp>

#include <StarTracker/Core/Application.hpp>
#include <StarTracker/Core/Assert.hpp>
#include <StarTracker/Core/Core.hpp>
#include <StarTracker/Core/Input.hpp>
#include <StarTracker/Core/Tracker.hpp>
#include <StarTracker/Utils/Serial/Package.hpp>
#include <StarTracker/Utils/Serial/Serial.hpp>

#include <StarTracker/Utils/GeoLocation.hpp>
#include <StarTracker/Utils/HttpRequest.hpp>
#include <StarTracker/Utils/Stopwatch.hpp>

#include <filesystem>
#include <iostream>
#include <thread>

namespace StarTracker {

	class TrackableBodyView final : public Core::View {

	private:
		Core::Tracker tracker;
		Utils::LocationService::Location observer;
		std::vector<std::shared_ptr<Ephemeris::CelestialBody>> celestialBodies;

	public:
		explicit TrackableBodyView(void* nativeWindowHandle) noexcept;

		virtual void OnInit() noexcept override;
		virtual void OnUpdate(float deltaTime) noexcept override;
		virtual void OnDestroy() noexcept override;
	};
}

#endif // STARTRACKER_TRACKABLEBODYVIEW_H
