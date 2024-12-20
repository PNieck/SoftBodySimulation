#include <simulation/model/model.hpp>


Model::Model(SpringGraph &&graph, const SimulationEnvironment &env):
    timedLoop(static_cast<int>(env.deltaT * 1000.f), [this] { simulation.Update(); }),
    simulation(std::move(graph), env)
{
}

