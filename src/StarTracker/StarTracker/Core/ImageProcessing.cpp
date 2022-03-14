#include "ImageProcessing.hpp"

namespace StarTracker::Core {

    bool ImageProcessing::Initialize() noexcept {

        stackFrameBuffer = std::make_shared<OpenGL::FrameBuffer>(100, 100);
        vertexArray = std::make_shared<Core::OpenGL::VertexArray>();
        vertexBuffer = std::make_shared<Core::OpenGL::VertexBuffer>();
        indexBuffer = std::make_shared<Core::OpenGL::IndexBuffer>();

        // Stacking shader from AssetDataBase
        stackShader = AssetDataBase::LoadShader("textureStackVertex.glsl", "textureStackFragment.glsl");

        // Fixed texture vertices
        const static std::array<OpenGL::TextureVertex, 4> vertices = {

                OpenGL::TextureVertex{ glm::vec3{ -1.0f, -1.0f, 0.0f }, glm::vec2{ 0.0f, 0.0f }},
                OpenGL::TextureVertex{ glm::vec3{  1.0f, -1.0f, 0.0f }, glm::vec2{ 1.0f, 0.0f }},
                OpenGL::TextureVertex{ glm::vec3{  1.0f,  1.0f, 0.0f }, glm::vec2{ 1.0f, 1.0f }},
                OpenGL::TextureVertex{ glm::vec3{ -1.0f,  1.0f, 0.0f }, glm::vec2{ 0.0f, 1.0f }},
        };

        // Fixed indices for drawing the rectangle
        const static std::array<std::uint32_t, 6> indices = {

                0, 1, 2, 2, 0, 3
        };

        // Buffer elements that match the Shader
        const static std::vector<Core::OpenGL::BufferElement> vertexBufferElements = {

                Core::OpenGL::BufferElement{ Core::OpenGL::ShaderDataType::Float3, "aPosition" },
                Core::OpenGL::BufferElement{ Core::OpenGL::ShaderDataType::Float2, "aTextureCoordinates" }
        };

        // BufferLayout containing the elements
        const static Core::OpenGL::BufferLayout vertexBufferLayout{ vertexBufferElements };

        // Set up the VertexArray
        vertexArray->Bind();
        vertexBuffer->SetLayout(vertexBufferLayout);
        vertexBuffer->SetData(vertices.data(), static_cast<std::uint32_t>(vertices.size() * sizeof(OpenGL::TextureVertex)));
        indexBuffer->SetData(indices.data(), static_cast<std::uint32_t>(indices.size()));
        vertexArray->SetIndexBuffer(indexBuffer);
        vertexArray->SetVertexBuffer(vertexBuffer);

        return stackFrameBuffer->IsValid();
    }

    const std::shared_ptr<OpenGL::FrameBuffer>& ImageProcessing::Stack(const std::vector<std::shared_ptr<OpenGL::Texture>> &textureList) noexcept {

        if(textureList.size() < 2) {

            ASSERT(false && "Invalid number of Textures for Stacking Operation!");
        }

        // find max texture Resolution
        const auto[textureWidth, textureHeight] = [&]() -> std::pair<std::int32_t, std::int32_t> {

            std::int32_t maxTextureWidth{ textureList.at(0)->GetWidth() };
            std::int32_t maxTextureHeight{ textureList.at(0)->GetHeight() };
            for (const auto& currentTexture : textureList) {

                if(currentTexture->GetWidth() > maxTextureWidth) {

                    maxTextureWidth = currentTexture->GetWidth();
                }
                if(currentTexture->GetHeight() > maxTextureHeight) {

                    maxTextureHeight = currentTexture->GetHeight();
                }
            }

            return { maxTextureWidth, maxTextureHeight };
        }();
        stackFrameBuffer->Resize(textureWidth, textureHeight);

        // Pass the number of textures to the Shader
        stackShader->SetInt("uNumberOfPassedTextures", static_cast<int>(textureList.size()));

        // Bind the Textures to their corresponding slot
        for (auto i = std::size_t{ 0 }; i < textureList.size(); i++) {

            const auto& currentTexture = textureList.at(i);
            currentTexture->Bind(i);

            const auto uniformName = std::format("uTextures[{}]", i);
            stackShader->SetInt(uniformName, static_cast<int>(i));
        }

        // Draw call which stacks the textures
        stackFrameBuffer->Bind();
        OpenGL::Renderer::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
        OpenGL::Renderer::Clear();
        OpenGL::Renderer::DrawIndexed(vertexArray, stackShader, OpenGL::PrimitiveMode::Triangle);
        stackFrameBuffer->Unbind();

        // TODO(Plank): Copy framebuffer content to a separate Texture
        return stackFrameBuffer;
    }

    const std::shared_ptr<OpenGL::FrameBuffer>& ImageProcessing::GetStackFrameBuffer() noexcept {

        return stackFrameBuffer;
    }
}