#pragma once

#include <glm/vec3.hpp>
#include "../utils/line.hpp"
#include "../utils/plane.hpp"


class BoxCollider {
public:
    explicit BoxCollider(const float wallLen, const float collisionDampingCoef):
        maxCoordinate(wallLen / 2.f), collisionDampingCoef(collisionDampingCoef) {}

    void Collide(glm::vec3& newPos, const glm::vec3& deltaPos, glm::vec3& newVel) const;
private:
    float maxCoordinate;
    float collisionDampingCoef;

    struct CollisionResult {
        glm::vec3 newPos;
        glm::vec3 newDeltaPos;
    };

    static glm::vec3 CalculateCollisionResult(glm::vec3& newPos, const glm::vec3& deltaPos, const Plane& wall);
};