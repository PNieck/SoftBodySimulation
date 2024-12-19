#pragma once

#include "shader.hpp"


class NormalsCheckShader: public Shader {
public:
    NormalsCheckShader():
        Shader(
            "../../shaders/passThroughShader.vert",
            "../../shaders/stdShader.frag",
            nullptr, // No tesselation control shader
            "../../shaders/bezierSurfaceShader.tese",
            "../../shaders/normals.geom"
        ) {}

    void SetColor(const glm::vec4& color) const
        { SetVec4("color", color); }

    void SetMVP(const glm::mat4& matrix) const
        { SetMatrix4("MVP", matrix); }

    void SetProjection(const glm::mat4& matrix) const
        { SetMatrix4("projection", matrix); }

    void SetModelMatrix(const glm::mat4& mtx) const
        { SetMatrix4("model", mtx); }

    void SetViewMatrix(const glm::mat4& mtx) const
        { SetMatrix4("view", mtx); }

};
