#pragma once

#include "shader.hpp"


class BicubicBezierSurfaceShader: public Shader {
public:
    BicubicBezierSurfaceShader():
        Shader(
            "../../shaders/passThroughShader.vert",
            "../../shaders/phong.frag",
            nullptr, // No tesselation control shader
            "../../shaders/bezierSurfaceShader.tese"
        ) {}

    void SetColor(const glm::vec4& color) const
        { SetVec4("fragmentColor", color); }

    // void SetMVP(const glm::mat4& matrix) const
    //     { SetMatrix4("MVP", matrix); }

    void SetLightPosition(const glm::vec3& position) const
        { SetVec3("lightPos", position); }

    void SetCameraPosition(const glm::vec3& position) const
        { SetVec3("camPos", position); }

    void SetProjection(const glm::mat4& matrix) const
        { SetMatrix4("projection", matrix); }

    void SetModelMatrix(const glm::mat4& mtx) const
        { SetMatrix4("model", mtx); }

    void SetViewMatrix(const glm::mat4& mtx) const
        { SetMatrix4("view", mtx); }
};
