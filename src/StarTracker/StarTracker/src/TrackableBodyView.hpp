#ifndef STARTRACKER_TRACKABLEBODYVIEW_H
#define STARTRACKER_TRACKABLEBODYVIEW_H

#include <StarAPI/StarAPI.hpp>

#include "core/Application.hpp"
#include "core/Assert.hpp"
#include "core/Core.hpp"
#include "core/Input.hpp"
#include "core/Tracker.hpp"
#include "utils/serial/Package.hpp"
#include "utils/serial/Serial.hpp"

#include "utils/GeoLocation.hpp"
#include "utils/HttpRequest.hpp"
#include "utils/Stopwatch.hpp"

#include <filesystem>
#include <iostream>
#include <thread>

namespace StarTracker {

	class TrackableBodyView : public Core::View {

	private:
		Ephemeris::Coordinates::Observer observer;
		Core::Tracker tracker;
		std::vector<std::shared_ptr<Ephemeris::CelestialBody>> celestialBodies;

	public:
		explicit TrackableBodyView(void* nativeWindowHandle) noexcept;

		virtual void OnInit() noexcept override;
		virtual void OnUpdate(float deltaTime) noexcept override;
		virtual void OnDestroy() noexcept override;
	};
}

#endif // STARTRACKER_TRACKABLEBODYVIEW_H
