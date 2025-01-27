#pragma once

#include "shader.hpp"


class GridShader: public Shader {
public:
    GridShader():
        Shader(
            "../../shaders/gridShader.vert",
            "../../shaders/gridShader.frag"
        ) {}

    void SetFarPlane(const float farPlane) const
        { SetFloat("far", farPlane); }

    void SetNearPlane(const float nearPlane) const
        { SetFloat("near", nearPlane); }

    void SetViewMatrix(const glm::mat4& viewMtx) const
        { SetMatrix4("view", viewMtx); }

    void SetProjectionMatrix(const glm::mat4& projMtx) const
        { SetMatrix4("projection", projMtx); }
};
