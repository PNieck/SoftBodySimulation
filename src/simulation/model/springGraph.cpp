#include <simulation/model/springGraph.hpp>


SpringGraph::SpringGraph(const size_t materialPointsNb, const size_t springsNb):
    neighbours(materialPointsNb)
{
    materialPoints.reserve(materialPointsNb);
    springs.reserve(springsNb);
}


MaterialPointId SpringGraph::AddMaterialPoint(const MaterialPoint &materialPoint) {
    materialPoints.emplace_back(materialPoint);

    return materialPoints.size() - 1;
}


SpringId SpringGraph::AddSpring(float restLength, float springCoef, MaterialPointId end1, MaterialPointId end2) {
    springs.emplace_back(end1, end2, restLength, springCoef);
    SpringId id = springs.size() - 1;

    neighbours[end1].emplace_back(end2, id);
    neighbours[end2].emplace_back(end1, id);

    return id;
}
