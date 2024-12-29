#pragma once

#include "springGraph.hpp"
#include "simulationEnvironment.hpp"

#include <mutex>


class SpringsSimulation {
public:
    SpringsSimulation(SpringGraph&& graph, const SimulationEnvironment& environment):
        springGraph(std::move(graph)),
        environment(environment),
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

private:
    SpringGraph springGraph;
    SimulationEnvironment environment;
    std::mutex simulationMutex;

    std::vector<glm::vec3> v;
    std::vector<glm::vec3> p;

    glm::vec3 ViscousDampingForce(const glm::vec3& v) const;
    glm::vec3 SpringForce(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& v1, const glm::vec3& v2, float springCoef, float springLen) const;

    static float DotI(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& v1, const glm::vec3& v2);
};
