#include "image-processing.hpp"
#include "graphics/renderer.hpp"

void ImageProcessing::Initialize() noexcept {
    vertexArray = std::make_shared<graphics::VertexArray>();
    vertexBuffer = std::make_shared<graphics::VertexBuffer>();
    indexBuffer = std::make_shared<graphics::IndexBuffer>();

    // Stacking shader from AssetDataBase
    stackShader = AssetDatabase::LoadShader("textureStackVertex.glsl", "textureStackFragment.glsl");
    kernelShader = AssetDatabase::LoadShader("kernelVertex.glsl", "kernelFragment.glsl");

    // Fixed texture vertices
    const static std::array vertices = {
        graphics::TextureVertex{ glm::vec3{ -1.0f, -1.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f },
                                 glm::vec2{ 0.0f, 0.0f } },
        graphics::TextureVertex{ glm::vec3{ 1.0f, -1.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f },
                                 glm::vec2{ 1.0f, 0.0f } },
        graphics::TextureVertex{ glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f },
                                 glm::vec2{ 1.0f, 1.0f } },
        graphics::TextureVertex{ glm::vec3{ -1.0f, 1.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f },
                                 glm::vec2{ 0.0f, 1.0f } }
    };

    // Fixed indices for drawing the rectangle
    const static std::array<u32, 6> indices = { 0, 1, 2, 2, 0, 3 };

    // Buffer elements that match the Shader
    const static std::vector vertexBufferElements = {
        graphics::BufferElement{ graphics::ShaderDataType::Float3, "aPosition" },
        graphics::BufferElement{ graphics::ShaderDataType::Float3, "aColor" },
        graphics::BufferElement{ graphics::ShaderDataType::Float2, "aTextureCoordinates" }
    };

    // BufferLayout containing the elements
    const static graphics::BufferLayout vertexBufferLayout{ vertexBufferElements };

    // Set up the VertexArray
    vertexArray->Bind();
    vertexBuffer->SetLayout(vertexBufferLayout);
    vertexBuffer->SetData(vertices.data(), static_cast<u32>(vertices.size() * sizeof(graphics::TextureVertex)));
    indexBuffer->SetData(indices.data(), static_cast<u32>(indices.size()));
    vertexArray->SetIndexBuffer(indexBuffer);
    vertexArray->SetVertexBuffer(vertexBuffer);
}

bool ImageProcessing::Stack(std::shared_ptr<graphics::FrameBuffer> target,
                            const std::vector<std::shared_ptr<graphics::Texture>>& textureList) noexcept {
    if (textureList.empty() || !target->IsValid()) {
        return false;
    }

    // find max texture Resolution
    const auto [textureWidth, textureHeight] = [&]() -> std::pair<s32, s32> {
        s32 maxTextureWidth{ textureList.at(0)->GetWidth() };
        s32 maxTextureHeight{ textureList.at(0)->GetHeight() };
        for (const auto& currentTexture : textureList) {
            if (currentTexture->GetWidth() > maxTextureWidth) {
                maxTextureWidth = currentTexture->GetWidth();
            }
            if (currentTexture->GetHeight() > maxTextureHeight) {
                maxTextureHeight = currentTexture->GetHeight();
            }
        }
        return { maxTextureWidth, maxTextureHeight };
    }();
    target->Resize(textureWidth, textureHeight);

    // Pass the number of textures to the Shader
    stackShader->SetInt("uNumberOfPassedTextures", static_cast<int>(textureList.size()));

    // Prepare pipeline
    target->Bind();
    graphics::Renderer::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    graphics::Renderer::Clear();

    // Bind the Textures to their corresponding slot
    usize index = 0;
    for (const auto& textureIterator : textureList) {
        const auto textureUniform = fmt::format("uTextures[{}]", index);
        textureIterator->Bind(static_cast<u32>(index));
        stackShader->SetInt(textureUniform, static_cast<int>(index));
        if (++index == 32) {
            // Submit Draw call
            graphics::Renderer::DrawIndexed(vertexArray, stackShader, graphics::PrimitiveMode::Triangle);
            index = 0;
        }
    }
    if (index != 0) {
        graphics::Renderer::DrawIndexed(vertexArray, stackShader, graphics::PrimitiveMode::Triangle);
    }
    target->Unbind();
    return true;
}

bool ImageProcessing::Kernel(std::shared_ptr<graphics::FrameBuffer> target,
                             std::shared_ptr<graphics::FrameBuffer> source,
                             const std::array<f32, 9>& kernel) noexcept {
    target->Resize(source->GetWidth(), source->GetHeight());

    for (usize i = 0; i < 9; i++) {
        const auto kernelElement = kernel.at(i);
        const auto uniformName = fmt::format("uKernel[{}]", i);
        kernelShader->SetFloat(uniformName, kernelElement);
    }

    kernelShader->SetInt("uTexture", 0);
    glBindTextureUnit(0, source->GetNativeTextureHandle());

    target->Bind();
    graphics::Renderer::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    graphics::Renderer::Clear();
    graphics::Renderer::DrawIndexed(vertexArray, kernelShader, graphics::PrimitiveMode::Triangle);
    target->Unbind();

    return true;
}
