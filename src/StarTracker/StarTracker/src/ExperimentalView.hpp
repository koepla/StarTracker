#ifndef STARTRACKER_EXPERIMENTALVIEW_H
#define STARTRACKER_EXPERIMENTALVIEW_H

#include <StarAPI/StarAPI.hpp>

#include "Core/Application.hpp"
#include "Core/Assert.hpp"
#include "Core/Core.hpp"
#include "Core/OpenGL/Shader.hpp"
#include "Core/OpenGL/VertexArray.hpp"
#include "Utils/Serial/Package.hpp"
#include "Utils/Serial/Serial.hpp"

#include "Utils/GeoLocation.hpp"
#include "Utils/HttpRequest.hpp"
#include "Utils/Stopwatch.hpp"

#include <filesystem>
#include <iostream>

namespace StarTracker {

	class ExperimentalView final : public Core::View {

	private:
		std::shared_ptr<Core::OpenGL::VertexArray> vertexArray;
		std::shared_ptr<Core::OpenGL::VertexBuffer> vertexBuffer;
		std::shared_ptr<Core::OpenGL::IndexBuffer> indexBuffer;
		std::shared_ptr<Core::OpenGL::Shader> shader;

	public:
		explicit ExperimentalView(void* nativeWindowHandle) noexcept;

		virtual void OnInit() noexcept override;
		virtual void OnUpdate(float deltaTime) noexcept override;
		virtual void OnDestroy() noexcept override;
	};
}


#endif // STARTRACKER_EXPERIMENTALVIEW_H

