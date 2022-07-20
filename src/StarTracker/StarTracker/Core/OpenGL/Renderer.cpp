#include "Renderer.hpp"

namespace StarTracker::Core::OpenGL {

    void Renderer::Initialize() noexcept {

        glEnable(GL_DEPTH_TEST);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    }

    void Renderer::Clear() noexcept {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::SetClearColor(const glm::vec4& color) noexcept {

        glClearColor(color.r, color.g, color.b, color.a);
    }

    void Renderer::DrawIndexed(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<Shader> shader, PrimitiveMode mode) noexcept {

        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);

        const auto indexCount = vertexArray->GetIndexBuffer()->GetIndexCount();

        shader->Bind();
        vertexArray->Bind();
        glDrawElements(static_cast<std::uint32_t>(mode), indexCount, GL_UNSIGNED_INT, nullptr);

        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_POLYGON_SMOOTH);
    }

    void Renderer::DrawModel(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader) noexcept {

        const auto& indexBuffer = model->GetIndexBuffer();
        const auto& vertexBuffer = model->GetVertexBuffer();
        const auto& vertexArray = model->GetVertexArray();
        const auto& texture = model->GetTexture();

        // In this case the Model has no texture
        if (texture != nullptr) {

            texture->Bind(0);
            shader->SetInt("uTexture", 0);
        }

        DrawIndexed(vertexArray, shader, PrimitiveMode::Triangle);
    }
}