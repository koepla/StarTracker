#ifndef STARTRACKER_CORE_IMAGEPROCESSING_H
#define STARTRACKER_CORE_IMAGEPROCESSING_H

#include <StarTracker/Core/AssetDataBase.hpp>
#include <StarTracker/Core/OpenGL/FrameBuffer.hpp>
#include <StarTracker/Core/OpenGL/Shader.hpp>
#include <StarTracker/Core/OpenGL/Texture.hpp>
#include <StarTracker/Core/OpenGL/VertexArray.hpp>
#include <StarTracker/Core/OpenGL/Renderer.hpp>

namespace StarTracker::Core {

	class ImageProcessing {

	private:
		static inline std::shared_ptr<OpenGL::VertexArray> vertexArray;
		static inline std::shared_ptr<OpenGL::VertexBuffer> vertexBuffer;
		static inline std::shared_ptr<OpenGL::IndexBuffer> indexBuffer;
		static inline std::shared_ptr<OpenGL::Shader> stackShader;
		static inline std::shared_ptr<OpenGL::Shader> kernelShader;

	private:
		/**
		* @brief Initializes the rendering pipeline
		* 
		*/
		static void initialize() noexcept;

	public:
		static inline constexpr std::array<float, 9> KernelBlur = { 0.0625f, 0.125f, 0.0625f, 0.125f, 0.5f, 0.125f, 0.0625f, 0.125f, 0.0625f };
		static inline constexpr std::array<float, 9> KernelEdgeDetection = { 1.0f, 1.0f, 1.0f, 1.0f, -8.0f, 1.0f, 1.0f, 1.0f, 1.0f };
		static inline constexpr std::array<float, 9> KernelSharpen = { 0.0f, -1.0f, 0.0f, -1.0f, 5.0f, -1.0f, 0.0f, -1.0f, 0.0f };

	public:
		/**
		* @brief Stacks the passed textureList into the target framebuffer
		*        Note that only 32 textures at a time are supported
		*
		* @param target target framebuffer where the textures are stacked into
		*
		* @param textureList list of the textures (max size is 32)
		*
		* @return true if the stacking was successful
		* 
		*/
		[[nodiscard]] static bool Stack(std::shared_ptr<OpenGL::FrameBuffer> target, const std::vector<std::shared_ptr<OpenGL::Texture>>& textureList) noexcept;

		/**
		* @brief Renders the source framebuffer into the target framebuffer and attaching a kernel-filter to it
		*
		* @param target target framebuffer where the kernelled source framebuffer is rendered into
		*
		* @param source source framebuffer
		* 
		* @param kernel float array for the kernel
		*
		* @return true if the kernelled render was successful
		* 
		*/
		[[nodiscard]] static bool Kernel(std::shared_ptr<OpenGL::FrameBuffer> target, std::shared_ptr<OpenGL::FrameBuffer> source, const std::array<float, 9>& kernel) noexcept;
	};
}

#endif // STARTRACKER_CORE_IMAGEPROCESSING_H