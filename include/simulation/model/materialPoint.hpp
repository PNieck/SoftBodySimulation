#pragma once

#include <glm/vec3.hpp>

using MaterialPointId = size_t;


class MaterialPoint {
public:
    MaterialPoint(const float mass, const glm::vec3& position):
        mass(mass), position(position), velocity(0.f) {}

    float mass;
    glm::vec3 position;
    glm::vec3 velocity;
};
