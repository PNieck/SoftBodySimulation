#pragma once

#include <optional>

#include <glm/glm.hpp>

#include "line.hpp"


class Plane {
public:
    Plane(const glm::vec3& pointOnPlane, const glm::vec3& perpendicularVector):
        perpendicularVec(normalize(perpendicularVector)), pointOnPlane(pointOnPlane) {}

    [[nodiscard]]
    std::optional<glm::vec3> Intersect(const Line& line) const;

    [[nodiscard]]
    const glm::vec3& Normal() const
        { return perpendicularVec; }

private:
    glm::vec3 perpendicularVec;
    glm::vec3 pointOnPlane;
};
