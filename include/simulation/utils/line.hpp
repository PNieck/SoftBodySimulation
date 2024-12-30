#pragma once

#include <glm/glm.hpp>

#include <cassert>


class Line {
public:
    static Line FromTwoPoints(const glm::vec3& point1, const glm::vec3& point2)
    { return { point1, point2 - point1 }; }

    Line(const glm::vec3& pointOnLine, const glm::vec3& dir);

    [[nodiscard]]
    glm::vec3 GetDirection() const { return direction; }

    [[nodiscard]]
    glm::vec3 GetSamplePoint() const { return pointOnLine; }

    [[nodiscard]]
    glm::vec3 GetPointOnLine(const float t) const
    { return t * direction + pointOnLine; }

    [[nodiscard]]
    glm::vec3 ProjectPointToLine(const glm::vec3& point) const;

private:
    glm::vec3 pointOnLine;
    glm::vec3 direction;
};
