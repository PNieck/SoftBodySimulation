#include <simulation/utils/line.hpp>


Line::Line(const glm::vec3 &pointOnLine, const glm::vec3 &dir):
    pointOnLine(pointOnLine),
    direction(normalize(dir))
{
    assert(dot(dir, dir) > 0.f);
}


glm::vec3 Line::ProjectPointToLine(const glm::vec3 &point) const
{
    const glm::vec3 v = point - GetSamplePoint();
    const float t = dot(v, GetDirection());

    return GetPointOnLine(t);
}
