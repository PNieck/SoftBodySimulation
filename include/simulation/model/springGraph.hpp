#pragma once

#include "materialPoint.hpp"
#include "spring.hpp"

#include <vector>


class SpringGraph {
public:
    using MaterialPointId = size_t;
    using SpringId = size_t;

    SpringGraph() = default;
    explicit SpringGraph(size_t materialPointsNb, size_t springsNb);

    MaterialPointId AddMaterialPoint(const MaterialPoint& materialPoint);

    SpringId AddSpring(float restLength, float springCoef, MaterialPointId end1, MaterialPointId end2);

    [[nodiscard]]
    const MaterialPoint& GetMaterialPoint(const MaterialPointId id) const
        { return materialPoints.at(id); }

    [[nodiscard]]
    MaterialPoint& GetMaterialPoint(const MaterialPointId id)
        { return materialPoints.at(id); }

    [[nodiscard]]
    const std::vector<MaterialPoint>& MaterialPoints() const
        { return materialPoints; }

    [[nodiscard]]
    const std::vector<Spring>& Springs() const
        { return springs; }

private:
    std::vector<MaterialPoint> materialPoints;
    std::vector<Spring> springs;
};
