#ifndef STARTRACKER_EXPERIMENTALVIEW_H
#define STARTRACKER_EXPERIMENTALVIEW_H

#include <StarTracker/Core/Core.hpp>
#include <StarTracker/Core/Application.hpp>
#include <StarTracker/Core/Assert.hpp>
#include <StarTracker/Core/AssetDataBase.hpp>
#include <StarTracker/Core/Input.hpp>
#include <StarTracker/Core/ImageProcessing.hpp>
#include <StarTracker/Core/OpenGL/Camera.hpp>
#include <StarTracker/Core/OpenGL/Shader.hpp>
#include <StarTracker/Core/OpenGL/VertexArray.hpp>
#include <StarTracker/Core/OpenGL/Texture.hpp>
#include <StarTracker/Core/OpenGL/FrameBuffer.hpp>
#include <StarTracker/Core/OpenGL/Renderer.hpp>
#include <StarTracker/Core/OpenGL/Model.hpp>
#include <StarTracker/Core/Logger.hpp>
		  
#include <StarTracker/Utils/GeoLocation.hpp>
#include <StarTracker/Utils/HttpRequest.hpp>
#include <StarTracker/Utils/Stopwatch.hpp>

namespace StarTracker {

	class ExperimentalView : public Core::View {

	private:
		bool isFocused;
		bool invertedTextureCoordinates;
		std::shared_ptr<Core::OpenGL::Camera> camera;
		std::shared_ptr<Core::OpenGL::Shader> shader;
		std::shared_ptr<Core::OpenGL::FrameBuffer> frameBuffer;
		std::shared_ptr<Core::OpenGL::Model> model;

	public:
		explicit ExperimentalView(void* nativeWindowHandle) noexcept;

		virtual void OnInit() noexcept override;
		virtual void OnUpdate(float deltaTime) noexcept override;
		virtual void OnDestroy() noexcept override;

	private:
		void reloadModel() noexcept;
		void drawModelInfoCard(const glm::vec2& size) noexcept;
	};
}

#endif // STARTRACKER_EXPERIMENTALVIEW_H