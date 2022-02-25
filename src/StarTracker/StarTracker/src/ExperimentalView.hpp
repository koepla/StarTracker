#ifndef STARTRACKER_EXPERIMENTALVIEW_H
#define STARTRACKER_EXPERIMENTALVIEW_H

#include "core/Application.hpp"
#include "core/Assert.hpp"
#include "core/Core.hpp"
#include "core/opengl/Shader.hpp"
#include "core/opengl/VertexArray.hpp"
#include "core/opengl/Texture.hpp"
#include "utils/serial/Package.hpp"
#include "utils/serial/Serial.hpp"
		  
#include "utils/GeoLocation.hpp"
#include "utils/HttpRequest.hpp"
#include "utils/Stopwatch.hpp"

#include <glm/glm.hpp>

#include <filesystem>
#include <iostream>

namespace StarTracker {

	class ExperimentalView final : public Core::View {

	private:
		std::shared_ptr<Core::OpenGL::VertexArray> vertexArray;
		std::shared_ptr<Core::OpenGL::VertexBuffer> vertexBuffer;
		std::shared_ptr<Core::OpenGL::IndexBuffer> indexBuffer;
		std::shared_ptr<Core::OpenGL::Shader> shader;
		std::shared_ptr<Core::OpenGL::Texture> texture;

	public:
		explicit ExperimentalView(void* nativeWindowHandle) noexcept;

		virtual void OnInit() noexcept override;
		virtual void OnUpdate(float deltaTime) noexcept override;
		virtual void OnDestroy() noexcept override;
	};
}

#endif // STARTRACKER_EXPERIMENTALVIEW_H