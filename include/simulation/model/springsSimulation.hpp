#pragma once

#include "springGraph.hpp"
#include "simulationEnvironment.hpp"

#include <mutex>


class SpringsSimulation {
public:
    SpringsSimulation(SpringGraph&& graph, const SimulationEnvironment& environment):
        actSpringGraph(std::move(graph)),
        prevSpringGraph(actSpringGraph),
        environment(environment),
        forces(actSpringGraph.MaterialPoints().size(), glm::vec3(0.f))
    {}

    void Update();

    const SpringGraph& StartReadingGraph() {
        simulationMutex.lock();
        return actSpringGraph;
    }

    void EndReadingGraph()
        { simulationMutex.unlock(); }


    std::tuple<SpringGraph&, SpringGraph&> StartWritingGraph() {
        simulationMutex.lock();
        return { actSpringGraph, prevSpringGraph };
    }

    void EndWritingGraph()
        { simulationMutex.unlock(); }

private:
    SpringGraph actSpringGraph;
    SpringGraph prevSpringGraph;

    SimulationEnvironment environment;

    std::vector<glm::vec3> forces;

    // std::vector<glm::vec3> prevVelocities;
    // std::vector<glm::vec3> velocities;
    //
    // std::vector<glm::vec3> prevPositions;
    // std::vector<glm::vec3> positions;

    std::mutex simulationMutex;

    void Forces();
    void MovePoints();
};
