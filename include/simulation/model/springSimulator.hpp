#pragma once

#include "springGraph.hpp"
#include "simulationEnvironment.hpp"


class SpringSimulator {
public:
    explicit SpringSimulator(SpringGraph&& graph, const SimulationEnvironment& environment = SimulationEnvironment()):
        springGraph(std::move(graph)), environment(environment) {}

    [[nodiscard]]
    const SpringGraph& GetSpringsState() const
        { return springGraph; }

private:
    SpringGraph springGraph;
    SimulationEnvironment environment;
};