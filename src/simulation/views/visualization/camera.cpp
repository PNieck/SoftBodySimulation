#include <simulation/views/visualization/camera.hpp>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <numbers>


Camera::Camera(const CameraParameters& cameraParameters):
    position(-1.f, 5.f, 0.f),
    up(0.f, 1.f, 0.f),
    right(0.f, 0.f, 1.f),
    front(1.f, 0.f, 0.f),
    parameters(cameraParameters)
{
}


glm::mat4 Camera::ViewMatrix() const
{
    const glm::vec3 direction = normalize(position - parameters.target);

    return lookAt(position, direction, up);
}


glm::mat4 Camera::ProjectionMatrix() const
{
    return glm::perspective(
        parameters.fov,
        parameters.GetAspectRatio(),
        parameters.nearPlane,
        parameters.farPlane
    );
}
