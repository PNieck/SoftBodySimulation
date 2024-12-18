#pragma once

#include "shader.hpp"


class BicubicBezierSurfaceShader: public Shader {
public:
    BicubicBezierSurfaceShader():
        Shader(
            "../../shaders/passThroughShader.vert",
            "../../shaders/stdShader.frag",
            nullptr, // No tesselation control shader
            "../../shaders/bezierSurfaceShader.tese"
        ) {}

    void SetColor(const glm::vec4& color) const
        { SetVec4("color", color); }

    void SetMVP(const glm::mat4& matrix) const
        { SetMatrix4("MVP", matrix); }
};
