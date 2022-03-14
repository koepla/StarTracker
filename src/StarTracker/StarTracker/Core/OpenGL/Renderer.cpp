#include "Renderer.hpp"

namespace StarTracker::Core::OpenGL {

    void Renderer::Clear() noexcept {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::SetClearColor(const glm::vec4 &color) noexcept {

        glClearColor(color.r, color.g, color.b, color.a);
    }

    void Renderer::DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray, const std::shared_ptr<Shader> &shader, PrimitiveMode mode) noexcept {

        const auto indexCount = vertexArray->GetIndexBuffer()->GetIndexCount();

        shader->Bind();
        vertexArray->Bind();
        glDrawElements(static_cast<std::uint32_t>(mode), indexCount, GL_UNSIGNED_INT, nullptr);
    }
}