#ifndef STARTRACKER_CORE_OPENGL_MODEL_H
#define STARTRACKER_CORE_OPENGL_MODEL_H

#include <StarTracker/Core/Core.hpp>
#include <StarTracker/Core/Assert.hpp>
#include <StarTracker/Core/OpenGL/Shader.hpp>
#include <StarTracker/Core/OpenGL/Texture.hpp>
#include <StarTracker/Core/OpenGL/VertexArray.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <filesystem>
#include <unordered_map>

namespace StarTracker::Core::OpenGL {

    class Model {

    private:
        bool hasTexture;
        std::filesystem::path filePath;
        std::shared_ptr<IndexBuffer> indexBuffer;
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<VertexArray> vertexArray;
        std::shared_ptr<Texture> texture;

    public:
        Model() noexcept;
        void LoadFromFile(const std::filesystem::path& filePath, const glm::vec3& color) noexcept;
        void LoadFromFile(const std::filesystem::path& filePath, const std::filesystem::path& texturePath) noexcept;

        [[nodiscard]] const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const noexcept;
        [[nodiscard]] const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const noexcept;
        [[nodiscard]] const std::shared_ptr<VertexArray>& GetVertexArray() const noexcept;
        [[nodiscard]] const std::shared_ptr<Texture>& GetTexture() const noexcept;
    };
}

#endif // STARTRACKER_CORE_OPENGL_MODEL_H