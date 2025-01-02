#pragma once

#include "shader.hpp"


class BezierCubeShader: public Shader {
public:
    BezierCubeShader():
        Shader(
            "../../shaders/bezierCubeShader.vert",
            "../../shaders/phong.frag"
        ) {}

    void SetCameraMatrix(const glm::mat4& cameraMatrix) const
        { SetMatrix4("cameraMtx", cameraMatrix); }

    void SetControlPoints(const glm::vec3* cps) const
        { SetVec3Array("controlPoints", cps, 64); }

    void SetColor(const glm::vec4& color) const
        { SetVec4("fragmentColor", color); }

    void SetLightPos(const glm::vec3& lightPos) const
        { SetVec3("lightPos", lightPos); }

    void SetCameraPos(const glm::vec3& cameraPos) const
        { SetVec3("camPos", cameraPos); }
};