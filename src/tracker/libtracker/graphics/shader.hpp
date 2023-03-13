#ifndef LIBTRACKER_CORE_OPENGL_SHADER_H
#define LIBTRACKER_CORE_OPENGL_SHADER_H

#include <libtracker/arch/file.hpp>
#include <libtracker/core/core.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <unordered_map>

namespace graphics {

    class Shader {
    public:
        Shader() noexcept;
        ~Shader() noexcept;

        /**
         * @brief Loads a vertex and fragment shader from the specified paths
         * @param vertexShaderPath Path to the vertex shader
         * @param fragmentShaderPath Path to the fragment shader
         */
        void LoadFromFile(const std::filesystem::path& vertexShaderPath,
                          const std::filesystem::path& fragmentShaderPath) noexcept;

        /**
         * @brief Sets the named integer uniform
         * @param name Name of the uniform
         * @param value Value that will be set
         */
        void SetInt(std::string_view name, int value) noexcept;

        /**
         * @brief Sets the named ivec2 uniform
         * @param name Name of the uniform
         * @param value Value that will be set
         */
        void SetInt2(std::string_view name, const glm::ivec2& value) noexcept;

        /**
         * @brief Sets the named ivec3 uniform
         * @param name Name of the uniform
         * @param value Value that will be set
         */
        void SetInt3(std::string_view name, const glm::ivec3& value) noexcept;

        /**
         * @brief Sets the named ivec4 uniform
         * @param name Name of the uniform
         * @param value Value that will be set
         */
        void SetInt4(std::string_view name, const glm::ivec4& value) noexcept;

        /**
         * @brief Sets the named f32 uniform
         * @param name Name of the uniform
         * @param value Value that will be set
         */
        void SetFloat(std::string_view name, f32 value) noexcept;

        /**
         * @brief Sets the named vec2 uniform
         * @param name Name of the uniform
         * @param value Value that will be set
         */
        void SetFloat2(std::string_view name, const glm::vec2& value) noexcept;

        /**
         * @brief Sets the named vec3 uniform
         * @param name Name of the uniform
         * @param value Value that will be set
         */
        void SetFloat3(std::string_view name, const glm::vec3& value) noexcept;

        /**
         * @brief Sets the named vec4 uniform
         * @param name Name of the uniform
         * @param value Value that will be set
         */
        void SetFloat4(std::string_view name, const glm::vec4& value) noexcept;

        /**
         * @brief Sets the named mat3 uniform
         * @param name Name of the uniform
         * @param value Value that will be set
         */
        void SetMat3(std::string_view name, const glm::mat3& value) noexcept;

        /**
         * @brief Sets the named mat4 uniform
         * @param name Name of the uniform
         * @param value Value that will be set
         */
        void SetMat4(std::string_view name, const glm::mat4& value) noexcept;

        /**
         * @brief Binds the shader
         */
        void Bind() const noexcept;

        /**
         * @brief Unbinds any shader
         */
        void Unbind() const noexcept;

    private:
        /**
         * @brief Gets the location of the specified uniform
         * @return OpenGL location of the uniform
         */
        s32 getUniformLocation(std::string_view name) noexcept;

    private:
        /**
         * @brief Compiles the specified shader and returns the OpenGL handle to it
         * @param sourceCode The actual source code of the shader
         * @param type The type of the shader [fragment | vertex]
         */
        static u32 compileShader(std::string_view sourceCode, u32 type) noexcept;

    private:
        u32 nativeHandle;
        std::unordered_map<std::string_view, s32> uniformLocationCache;
    };
}// namespace graphics

#endif// LIBTRACKER_CORE_OPENGL_SHADER_H