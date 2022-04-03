#ifndef STARTRACKER_IMAGEPROCESSINGVIEW_H
#define STARTRACKER_IMAGEPROCESSINGVIEW_H

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

#include <StarTracker/Utils/GeoLocation.hpp>
#include <StarTracker/Utils/HttpRequest.hpp>
#include <StarTracker/Utils/Stopwatch.hpp>
#include <StarTracker/Utils/File.hpp>

namespace StarTracker {

	class ImageProcessingView : public Core::View {

	private:
		std::shared_ptr<Core::OpenGL::FrameBuffer> stackFrameBuffer;
		std::shared_ptr<Core::OpenGL::FrameBuffer> kernelFrameBuffer;
		std::vector<std::shared_ptr<Core::OpenGL::Texture>> textureList;

	public:
		explicit ImageProcessingView(void* nativeWindowHandle) noexcept;

		virtual void OnInit() noexcept override;
		virtual void OnUpdate(float deltaTime) noexcept override;
		virtual void OnDestroy() noexcept override;
	};
}

#endif // STARTRACKER_IMAGEPROCESSINGVIEW_H