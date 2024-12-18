#include "simulation/model/springsSimulation.hpp"

#include <glm/glm.hpp>


void SpringsSimulation::Update() {
    std::lock_guard guard(simulationMutex);

    Forces();

    swap(actSpringGraph, prevSpringGraph);

    MovePoints();
}


void SpringsSimulation::Forces() {
    std::fill(forces.begin(), forces.end(), glm::vec3(0.f));

    int i=0;
    for (const auto& actSpring: actSpringGraph.Springs()) {
        const Spring& prevSpring = prevSpringGraph.GetSpring(i++);

        const MaterialPoint& actEnd1 = actSpringGraph.GetMaterialPoint(actSpring.anchorPoint1);
        const MaterialPoint& actEnd2 = actSpringGraph.GetMaterialPoint(actSpring.anchorPoint2);

        const MaterialPoint& prevEnd1 = prevSpringGraph.GetMaterialPoint(prevSpring.anchorPoint1);
        const MaterialPoint& prevEnd2 = prevSpringGraph.GetMaterialPoint(prevSpring.anchorPoint2);

        const float prevSpringExtension = length(prevEnd1.position - prevEnd2.position) - actSpring.restLength;
        const float actSpringExtension = length(actEnd1.position - actEnd2.position) - actSpring.restLength;

        const float forceVal = environment.dampingCoef * (actSpringExtension - prevSpringExtension)/environment.deltaT +
                               actSpring.springCoef * actSpringExtension;

        if (actEnd2.position != actEnd1.position) {
            glm::vec3 direction = normalize(actEnd2.position - actEnd1.position);

            forces.at(actSpring.anchorPoint1) += direction * forceVal;
            forces.at(actSpring.anchorPoint2) += -direction * forceVal;
        }
    }
}


void SpringsSimulation::MovePoints() {
    for (MaterialPointId id=0; id < forces.size(); id++) {
        MaterialPoint& point = actSpringGraph.GetMaterialPoint(id);

        if (point.mass != std::numeric_limits<float>::infinity()) {
            const MaterialPoint& prevPoint = prevSpringGraph.GetMaterialPoint(id);

            const glm::vec3 acceleration = forces.at(id) / point.mass;
            const glm::vec3 deltaV = acceleration * environment.deltaT;

            point.velocity = prevPoint.velocity + environment.deltaT * deltaV;
            point.position = prevPoint.position + prevPoint.velocity * environment.deltaT;
        }

    }
}
