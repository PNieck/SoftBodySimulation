#include <simulation/model/boxCollider.hpp>

#include <simulation/utils/line.hpp>
#include <simulation/utils/plane.hpp>


void BoxCollider::Collide(glm::vec3 &newPos, const glm::vec3 &deltaPos, glm::vec3 &newVel) const
{
    if (newPos.x > maxCoordinate) {
        const Plane wall(glm::vec3(maxCoordinate, 0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f));
        const glm::vec3 newDeltaPos = CalculateCollisionResult(newPos, deltaPos, wall);

        newVel.x = -newVel.x * collisionDampingCoef;
        Collide(newPos, newDeltaPos, newVel);
    }
    else if (newPos.x < -maxCoordinate) {
        const Plane wall(glm::vec3(-maxCoordinate, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
        const glm::vec3 newDeltaPos = CalculateCollisionResult(newPos, deltaPos, wall);

        newVel.x = -newVel.x * collisionDampingCoef;
        Collide(newPos, newDeltaPos, newVel);
    }
    else if (newPos.y > maxCoordinate) {
        const Plane wall(glm::vec3(0.f, maxCoordinate, 0.f), glm::vec3(0.f, -1.f, 0.f));
        const glm::vec3 newDeltaPos = CalculateCollisionResult(newPos, deltaPos, wall);

        newVel.y = -newVel.y * collisionDampingCoef;
        Collide(newPos, newDeltaPos, newVel);
    }
    else if (newPos.y < -maxCoordinate) {
        const Plane wall(glm::vec3(0.f, -maxCoordinate, 0.f), glm::vec3(0.f, 1.f, 0.f));
        const glm::vec3 newDeltaPos = CalculateCollisionResult(newPos, deltaPos, wall);

        newVel.y = -newVel.y * collisionDampingCoef;
        Collide(newPos, newDeltaPos, newVel);
    }
    else if (newPos.z > maxCoordinate) {
        const Plane wall(glm::vec3(0.f, 0.f, maxCoordinate), glm::vec3(0.f, 0.f, -1.f));
        const glm::vec3 newDeltaPos = CalculateCollisionResult(newPos, deltaPos, wall);

        newVel.z = -newVel.z * collisionDampingCoef;
        Collide(newPos, newDeltaPos, newVel);
    }
    else if (newPos.z < -maxCoordinate) {
        const Plane wall(glm::vec3(0.f, 0.f, -maxCoordinate), glm::vec3(0.f, 0.f, 1.f));
        const glm::vec3 newDeltaPos = CalculateCollisionResult(newPos, deltaPos, wall);

        newVel.z = -newVel.z * collisionDampingCoef;
        Collide(newPos, newDeltaPos, newVel);
    }
}


glm::vec3 BoxCollider::CalculateCollisionResult(glm::vec3 &newPos, const glm::vec3 &deltaPos, const Plane &wall) {
    const Line pointTrajectory(newPos - deltaPos, newPos);

    const glm::vec3 collisionPoint = wall.Intersect(pointTrajectory).value();
    const glm::vec3 reflected = reflect(pointTrajectory.GetDirection(), wall.Normal());

    const glm::vec3 newDeltaPos = newPos - collisionPoint;
    newPos = reflected * length(newDeltaPos) + collisionPoint;

    return newDeltaPos;
}

