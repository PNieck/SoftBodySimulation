#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>


class CameraParameters {
public:
    glm::vec3 target;
    int viewportWidth;
    int viewportHeight;
    float fov;          // Vertical fov
    float nearPlane;
    float farPlane;

    [[nodiscard]]
    float GetAspectRatio() const
        { return static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight); }
};


class Camera {
public:
    explicit Camera(const CameraParameters& cameraParameters);

    [[nodiscard]]
    glm::mat4 ViewMatrix() const;

    [[nodiscard]]
    glm::mat4 ProjectionMatrix() const;

    [[nodiscard]]
    glm::vec3 GetPosition() const
        { return position; }

    void SetPosition(const glm::vec3& pos)
        { position = pos; }

    void SetViewportSize(const int width, const int height)
        { parameters.viewportWidth = width; parameters.viewportHeight = height; }

private:
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 front;

    CameraParameters parameters;
};