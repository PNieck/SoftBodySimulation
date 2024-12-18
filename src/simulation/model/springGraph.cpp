#include <simulation/model/springGraph.hpp>


SpringGraph::SpringGraph(const size_t materialPointsNb, const size_t springsNb) {
    materialPoints.reserve(materialPointsNb);
    springs.reserve(springsNb);
}


MaterialPointId SpringGraph::AddMaterialPoint(const MaterialPoint &materialPoint) {
    materialPoints.emplace_back(materialPoint);

    return materialPoints.size() - 1;
}


SpringId SpringGraph::AddSpring(float restLength, float springCoef, MaterialPointId end1, MaterialPointId end2) {
    springs.emplace_back(end1, end2, restLength, springCoef);

    return springs.size() - 1;
}
