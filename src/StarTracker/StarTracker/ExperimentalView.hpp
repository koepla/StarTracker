#ifndef STARTRACKER_EXPERIMENTALVIEW_H
#define STARTRACKER_EXPERIMENTALVIEW_H

#include <StarTracker/Core/Application.hpp>
#include <StarTracker/Core/Assert.hpp>
#include <StarTracker/Core/Core.hpp>
#include <StarTracker/Core/Input.hpp>
#include <StarTracker/Core/OpenGL/Camera.hpp>
#include <StarTracker/Core/OpenGL/Shader.hpp>
#include <StarTracker/Core/OpenGL/VertexArray.hpp>
#include <StarTracker/Core/OpenGL/Texture.hpp>
#include <StarTracker/Core/OpenGL/FrameBuffer.hpp>
#include <StarTracker/Utils/Serial/Package.hpp>
#include <StarTracker/Utils/Serial/Serial.hpp>
		  
#include <StarTracker/Utils/GeoLocation.hpp>
#include <StarTracker/Utils/HttpRequest.hpp>
#include <StarTracker/Utils/Stopwatch.hpp>

#include <glm/glm.hpp>

#include <filesystem>
#include <iostream>

namespace StarTracker {

	class ExperimentalView : public Core::View {

	private:
        std::shared_ptr<Core::OpenGL::Camera> camera;
		std::shared_ptr<Core::OpenGL::VertexArray> vertexArray;
		std::shared_ptr<Core::OpenGL::VertexBuffer> vertexBuffer;
		std::shared_ptr<Core::OpenGL::IndexBuffer> indexBuffer;
		std::shared_ptr<Core::OpenGL::Shader> shader;
        std::shared_ptr<Core::OpenGL::FrameBuffer> frameBuffer;

        glm::mat4 transform;

	public:
		explicit ExperimentalView(void* nativeWindowHandle) noexcept;

		virtual void OnInit() noexcept override;
		virtual void OnUpdate(float deltaTime) noexcept override;
		virtual void OnDestroy() noexcept override;
	};
}

#endif // STARTRACKER_EXPERIMENTALVIEW_H