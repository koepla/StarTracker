#ifndef STARTRACKER_EXPERIMENTALVIEW_H
#define STARTRACKER_EXPERIMENTALVIEW_H

#include <StarAPI/StarAPI.hpp>

#include "core/Application.hpp"
#include "core/Assert.hpp"
#include "core/Core.hpp"
#include "utils/serial/Package.hpp"
#include "utils/serial/Serial.hpp"

#include "utils/GeoLocation.hpp"
#include "utils/HttpRequest.hpp"
#include "utils/Stopwatch.hpp"

#include <filesystem>
#include <iostream>

namespace StarTracker {

	class ExperimentalView : public Core::View {

	private:


	public:
		explicit ExperimentalView(void* nativeWindowHandle) noexcept;

		virtual void OnInit() noexcept override;
		virtual void OnUpdate(float deltaTime) noexcept override;
		virtual void OnDestroy() noexcept override;
	};
}


#endif // STARTRACKER_EXPERIMENTALVIEW_H

