#pragma once

#include "springsSimulation.hpp"
#include "timedLoop.hpp"


class Model {
public:
    Model(SpringGraph&& graph, const SimulationEnvironment& env);

    void StartSimulation()
        { timedLoop.Start(); }

    void EndSimulation()
        { timedLoop.End(); }

    void UpdateSimulation()
        { simulation.Update(); }

    [[nodiscard]]
    bool IsSimulationRunning() const
        { return timedLoop.IsRunning(); }

    const SpringGraph& StartReadingGraph()
        { return simulation.StartReadingGraph(); }

    void EndReadingGraph()
        { simulation.EndReadingGraph(); }

    std::tuple<SpringGraph&, SpringGraph&> StartWritingGraph()
        { return simulation.StartWritingGraph(); }

    void EndWritingGraph()
        { simulation.EndWritingGraph(); }
private:
    TimedLoop timedLoop;
    SpringsSimulation simulation;
};
