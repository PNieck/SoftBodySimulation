#pragma once

#include "springGraph.hpp"
#include "simulationEnvironment.hpp"
#include "boxCollider.hpp"

#include <mutex>


class SpringsSimulation {
public:
    SpringsSimulation(SpringGraph&& graph, const SimulationEnvironment& environment):
        springGraph(std::move(graph)),
        environment(environment),
        boxCollider(environment.simulationAreaEdgeLength, environment.collisionDamping),
        v(springGraph.MaterialPoints().size(), glm::vec3(0.f)),
        p(springGraph.MaterialPoints().size(), glm::vec3(0.f))
    {}

    void UpdateEuler();
    void UpdateRungeKutta();

    const SpringGraph& StartReadingGraph() {
        simulationMutex.lock();
        return springGraph;
    }

    void EndReadingGraph()
        { simulationMutex.unlock(); }


    SpringGraph& StartWritingGraph() {
        simulationMutex.lock();
        return springGraph;
    }

    void EndWritingGraph()
        { simulationMutex.unlock(); }

    void SetSimulationEnv(const SimulationEnvironment& environment);

    const SimulationEnvironment& GetSimulationEnv() const
        { return environment; }

private:
    SpringGraph springGraph;
    SimulationEnvironment environment;
    BoxCollider boxCollider;
    std::mutex simulationMutex;

    std::vector<glm::vec3> v;
    std::vector<glm::vec3> p;

    [[nodiscard]]
    glm::vec3 ViscousDampingForce(const glm::vec3& v) const;

    [[nodiscard]]
    glm::vec3 SpringForce(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& v1, const glm::vec3& v2, float springCoef, float springLen) const;

    static float DotI(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& v1, const glm::vec3& v2);

    void UpdatePositionsAndVelocities();
};
