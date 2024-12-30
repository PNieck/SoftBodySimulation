#include <simulation/utils/plane.hpp>


std::optional<glm::vec3> Plane::Intersect(const Line& line) const
{
    const float dotWithDir = dot(perpendicularVec, line.GetDirection());

    if (dotWithDir == 0.0f) {
        return std::nullopt;
    }

    const float dotWithLinePt = dot(perpendicularVec, line.GetSamplePoint());
    const float dotWithPlanePt = dot(perpendicularVec, pointOnPlane);

    const float t = (dotWithPlanePt - dotWithLinePt) / dotWithDir;

    return { line.GetPointOnLine(t) };
}
