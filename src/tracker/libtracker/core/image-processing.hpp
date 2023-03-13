#ifndef LIBTRACKER_CORE_IMAGEPROCESSING_H
#define LIBTRACKER_CORE_IMAGEPROCESSING_H

#include "asset-database.hpp"
#include "graphics/buffer.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "utility/types.hpp"

class ImageProcessing {
public:
    static inline constexpr std::array<f32, 9> KernelBlur = { 0.0625f, 0.125f,  0.0625f, 0.125f, 0.5f,
                                                              0.125f,  0.0625f, 0.125f,  0.0625f };
    static inline constexpr std::array<f32, 9> KernelEdgeDetection = { 1.0f, 1.0f, 1.0f, 1.0f, -8.0f,
                                                                       1.0f, 1.0f, 1.0f, 1.0f };
    static inline constexpr std::array<f32, 9> KernelSharpen = { 0.0f,  -1.0f, 0.0f,  -1.0f, 5.0f,
                                                                 -1.0f, 0.0f,  -1.0f, 0.0f };

    /**
     * @brief Initializes the rendering pipeline
     */
    static void Initialize() noexcept;

    /**
     * @brief Stacks the passed textureList into the target frame buffer
     *        Note that only 32 textures at a time are supported
     * @param target target frame buffer where the textures are stacked into
     * @param textureList list of the textures (max size is 32)
     * @return true if the stacking was successful
     */
    static bool Stack(std::shared_ptr<graphics::FrameBuffer> target,
                      const std::vector<std::shared_ptr<graphics::Texture>>& textureList) noexcept;

    /**
     * @brief Renders the source frame buffer into the target frame buffer and attaching a kernel-filter to it
     * @param target target frame buffer where the kernel-ed source frame buffer is rendered into
     * @param source source frame buffer
     * @param kernel f32 array for the kernel
     * @return true if the kernel-ed render was successful
     */
    static bool Kernel(std::shared_ptr<graphics::FrameBuffer> target,
                       std::shared_ptr<graphics::FrameBuffer> source,
                       const std::array<f32, 9>& kernel) noexcept;

private:
    static inline std::shared_ptr<graphics::VertexArray> vertexArray;
    static inline std::shared_ptr<graphics::VertexBuffer> vertexBuffer;
    static inline std::shared_ptr<graphics::IndexBuffer> indexBuffer;
    static inline std::shared_ptr<graphics::Shader> stackShader;
    static inline std::shared_ptr<graphics::Shader> kernelShader;
};

#endif// LIBTRACKER_CORE_IMAGEPROCESSING_H