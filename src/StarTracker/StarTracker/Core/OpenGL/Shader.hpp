#ifndef STARTRACKER_CORE_OPENGL_SHADER_H
#define STARTRACKER_CORE_OPENGL_SHADER_H

#include <StarTracker/Core/Assert.hpp>
#include <StarTracker/Utils/File.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <unordered_map>

namespace StarTracker::Core::OpenGL {

    class Shader {

    private:
        std::uint32_t nativeHandle;
        std::unordered_map<std::string_view, std::int32_t> uniformLocationCache;

    public:
        Shader() noexcept;
        ~Shader() noexcept;

        void LoadFromFile(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath) noexcept;

        void SetInt(std::string_view name, int value) noexcept;
        void SetInt2(std::string_view name, const glm::ivec2& value) noexcept;
        void SetInt3(std::string_view name, const glm::ivec3& value) noexcept;
        void SetInt4(std::string_view name, const glm::ivec4& value) noexcept;
        void SetFloat(std::string_view name, float value) noexcept;
        void SetFloat2(std::string_view name, const glm::vec2& value) noexcept;
        void SetFloat3(std::string_view name, const glm::vec3& value) noexcept;
        void SetFloat4(std::string_view name, const glm::vec4& value) noexcept;
        void SetMat3(std::string_view name, const glm::mat3& value) noexcept;
        void SetMat4(std::string_view name, const glm::mat4& value) noexcept;

        void Bind() const noexcept;
        void Unbind() const noexcept;

    private:
        [[nodiscard]] std::int32_t getUniformLocation(std::string_view name) noexcept;

    private:
        [[nodiscard]] static std::uint32_t compileShader(std::string_view sourceCode, std::uint32_t type) noexcept;
    };
}

#endif // STARTRACKER_CORE_OPENGL_SHADER_H