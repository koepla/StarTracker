#ifndef STARTRACKER_TRACKABLEBODYVIEW_H
#define STARTRACKER_TRACKABLEBODYVIEW_H

#include <StarAPI/StarAPI.hpp>

#include "Core/Application.hpp"
#include "Core/Assert.hpp"
#include "Core/Core.hpp"
#include "Core/Input.hpp"
#include "Core/Tracker.hpp"
#include "Utils/Serial/Package.hpp"
#include "Utils/Serial/Serial.hpp"

#include "Utils/GeoLocation.hpp"
#include "Utils/HttpRequest.hpp"
#include "Utils/Stopwatch.hpp"

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
