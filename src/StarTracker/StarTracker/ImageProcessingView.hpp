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
		struct Filter {

			std::string Name;
			std::shared_ptr<Core::OpenGL::FrameBuffer> Target;
		};

	private:
		std::vector<Filter> filterList;
		std::vector<std::shared_ptr<Core::OpenGL::Texture>> textureList;

		// FrameBuffer for rendering
		std::shared_ptr<Core::OpenGL::FrameBuffer> renderFrameBuffer;
		std::shared_ptr<Core::OpenGL::Shader> copyShader;

		// OpenGL Buffers for showing a specific Texture
		std::shared_ptr<Core::OpenGL::VertexArray> renderVertexArray;
		std::shared_ptr<Core::OpenGL::VertexBuffer> renderVertexBuffer;
		std::shared_ptr<Core::OpenGL::IndexBuffer> renderIndexBuffer;

	public:
		explicit ImageProcessingView(void* nativeWindowHandle) noexcept;

		virtual void OnInit() noexcept override;
		virtual void OnUpdate(float deltaTime) noexcept override;
		virtual void OnDestroy() noexcept override;

	private:
		void initializeBuffers() noexcept;
		void drawTextureListPanel() noexcept;
		void drawRenderFrameBufferPanel() noexcept;
		void drawFilterPanel() noexcept;
		void drawFilterHierarchyPanel() noexcept;
		void drawFilterKernel() noexcept;

		void showTexture(std::uint32_t textureNativeHandle, const glm::vec2& size) noexcept;
	};
}

#endif // STARTRACKER_IMAGEPROCESSINGVIEW_H