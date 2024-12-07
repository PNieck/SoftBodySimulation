#pragma once

#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <string>


class Shader {
public:
    unsigned int id;

    Shader(
        const char* vertexPath,
        const char* fragmentPath,
        const char* tesselationControlPath = nullptr,
        const char* tesselationEvaluationPath = nullptr,
        const char* geometryPath = nullptr
    );
    Shader(Shader&) = delete;
    Shader(const Shader&) = delete;

    ~Shader();

    void Use() const
        { glUseProgram(id); }

    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec3(const std::string& name, const glm::vec3& vec) const;
    void SetVec4(const std::string& name, const glm::vec4& vec) const;
    void SetMatrix4(const std::string &name, const glm::mat4& matrix) const;

    Shader& operator=(Shader&) = delete;
private:
    [[nodiscard]]
    int FindUniformLocation(const std::string &name) const;

    static unsigned int CompileSingleShader(const char* path, GLenum shaderType);
};


class UniformNotFoundInShader final : std::exception {
public:
    [[nodiscard]]
    const char * what() const noexcept override;
};
