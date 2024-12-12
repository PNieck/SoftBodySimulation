#pragma once

#include "shader.hpp"


class PhongShader: public Shader {
public:
    PhongShader():
        Shader(
            "../../shaders/phong.vert",
            "../../shaders/phong.frag"
        ) {}

    void SetColor(const glm::vec4& color) const
        { SetVec4("fragmentColor", color); }

    void SetLightPosition(const glm::vec3& position) const
        { SetVec3("lightPos", position); }

    void SetCameraPosition(const glm::vec3& position) const
        { SetVec3("camPos", position); }

    void SetModelMatrix(const glm::mat4& mtx) const
        { SetMatrix4("modelMtx", mtx); }

    void SetModelMatrixInverse(const glm::mat4& mtx) const
        { SetMatrix4("modelInvMtx", mtx); }

    void SetViewMatrix(const glm::mat4& mtx) const
        { SetMatrix4("viewMtx", mtx); }

    void SetProjectionMatrix(const glm::mat4& mtx) const
        { SetMatrix4("projectionMtx", mtx); }
};
