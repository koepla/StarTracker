#include "shader.hpp"

#include <libtracker/core/core.hpp>

namespace graphics {

    Shader::Shader() noexcept : nativeHandle{} { }

    Shader::~Shader() noexcept {
        glDeleteShader(nativeHandle);
    }

    void Shader::LoadFromFile(const std::filesystem::path& vertexShaderPath,
                              const std::filesystem::path& fragmentShaderPath) noexcept {
        const auto vertexShaderSource = arch::ReadFile(vertexShaderPath);
        const auto fragmentShaderSource = arch::ReadFile(fragmentShaderPath);

        const auto vertexShaderProgram = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
        const auto fragmentShaderProgram = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

        const auto shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShaderProgram);
        glAttachShader(shaderProgram, fragmentShaderProgram);
        glLinkProgram(shaderProgram);

        s32 linkSuccess{};
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);

        if (!linkSuccess) {
            s32 infoLength{};
            std::vector<char> failureInfo{};
            glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLength);
            failureInfo.resize(infoLength);
            glGetProgramInfoLog(shaderProgram, infoLength, nullptr, failureInfo.data());
            glDeleteProgram(shaderProgram);
            glDeleteProgram(vertexShaderProgram);
            glDeleteProgram(fragmentShaderProgram);
            LIBTRACKER_ASSERT(false, "Shader linking failed: {}", failureInfo.data());
        }

        s32 uniformCount{};
        glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &uniformCount);

        s32 maxUniformLength{};
        glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLength);

        if (uniformCount > 0 && maxUniformLength > 0) {
            std::vector<char> uniformNameBuffer{};
            uniformNameBuffer.resize(maxUniformLength);

            for (s32 i = 0; i < uniformCount; i++) {
                s32 length{};
                s32 size{};
                u32 dataType{};

                glGetActiveUniform(shaderProgram, i, maxUniformLength, &length, &size, &dataType,
                                   uniformNameBuffer.data());
                const auto location = glGetUniformLocation(shaderProgram, uniformNameBuffer.data());
                uniformLocationCache[uniformNameBuffer.data()] = location;

                LIBTRACKER_INFO("[{}, {}] Uniform {} has location {}", vertexShaderPath.filename().string(),
                                fragmentShaderPath.filename().string(), uniformNameBuffer.data(), location);
            }
        }
        nativeHandle = shaderProgram;
    }

    void Shader::SetInt(std::string_view name, int value) noexcept {
        glUseProgram(nativeHandle);
        glUniform1i(getUniformLocation(name), value);
    }

    void Shader::SetInt2(std::string_view name, const glm::ivec2& value) noexcept {
        glUseProgram(nativeHandle);
        glUniform2i(getUniformLocation(name), value.x, value.y);
    }

    void Shader::SetInt3(std::string_view name, const glm::ivec3& value) noexcept {
        glUseProgram(nativeHandle);
        glUniform3i(getUniformLocation(name), value.x, value.y, value.z);
    }

    void Shader::SetInt4(std::string_view name, const glm::ivec4& value) noexcept {
        glUseProgram(nativeHandle);
        glUniform4i(getUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void Shader::SetFloat(std::string_view name, f32 value) noexcept {
        glUseProgram(nativeHandle);
        glUniform1f(getUniformLocation(name), value);
    }

    void Shader::SetFloat2(std::string_view name, const glm::vec2& value) noexcept {
        glUseProgram(nativeHandle);
        glUniform2f(getUniformLocation(name), value.x, value.y);
    }

    void Shader::SetFloat3(std::string_view name, const glm::vec3& value) noexcept {
        glUseProgram(nativeHandle);
        glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
    }

    void Shader::SetFloat4(std::string_view name, const glm::vec4& value) noexcept {
        glUseProgram(nativeHandle);
        glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void Shader::SetMat3(std::string_view name, const glm::mat3& value) noexcept {
        glUseProgram(nativeHandle);
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::SetMat4(std::string_view name, const glm::mat4& value) noexcept {
        glUseProgram(nativeHandle);
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::Bind() const noexcept {
        glUseProgram(nativeHandle);
    }

    void Shader::Unbind() const noexcept {
        glUseProgram(0);
    }

    s32 Shader::getUniformLocation(std::string_view name) noexcept {
        if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
            return uniformLocationCache[name];
        }

        const auto location = glGetUniformLocation(nativeHandle, name.data());
        if (location == -1) {
            LIBTRACKER_ASSERT(false, "Uniform {} doesn't exist", name);
        } else {
            uniformLocationCache[name] = location;
        }

        return location;
    }

    u32 Shader::compileShader(std::string_view sourceCode, u32 type) noexcept {
        const auto shaderProgram = glCreateShader(type);
        const GLchar* shaderSourceCode = sourceCode.data();

        glShaderSource(shaderProgram, 1, &shaderSourceCode, nullptr);
        glCompileShader(shaderProgram);

        s32 compileSuccess{};
        glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &compileSuccess);

        if (!compileSuccess) {
            s32 infoLength{};
            std::vector<char> failureInfo{};
            glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLength);
            failureInfo.resize(infoLength);
            glGetShaderInfoLog(shaderProgram, infoLength, &infoLength, failureInfo.data());
            glDeleteProgram(shaderProgram);

            LIBTRACKER_ASSERT(false, "Shader compilation failed: {}", failureInfo.data());
        }

        return shaderProgram;
    }
}// namespace graphics