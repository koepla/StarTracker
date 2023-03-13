#include "renderer.hpp"

namespace graphics {

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

    void Renderer::DrawIndexed(std::shared_ptr<VertexArray> vertexArray,
                               std::shared_ptr<Shader> shader,
                               PrimitiveMode mode) noexcept {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);

        const auto indexCount = vertexArray->GetIndexBuffer()->GetIndexCount();

        shader->Bind();
        vertexArray->Bind();
        glDrawElements(static_cast<GLenum>(mode), static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);

        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_POLYGON_SMOOTH);
    }
}// namespace graphics