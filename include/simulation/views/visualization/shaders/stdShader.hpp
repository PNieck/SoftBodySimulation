#pragma once

#include "shader.hpp"


class StdShader: public Shader {
public:
    StdShader():
        Shader(
            "../../shaders/stdShader.vert",
            "../../shaders/stdShader.frag"
        ) {}

    void SetColor(const glm::vec4& color) const
        { SetVec4("color", color); }

    void SetMVP(const glm::mat4& matrix) const
        { SetMatrix4("MVP", matrix); }
};
