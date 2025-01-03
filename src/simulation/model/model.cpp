#include <simulation/model/model.hpp>


Model::Model(SpringGraph &&graph, const SimulationEnvironment &env):
    timedLoop(static_cast<int>(env.deltaT * 1000.f), [this] { simulation.UpdateRungeKutta2(); }),
    simulation(std::move(graph), env)
{
}


void Model::SetEnvironment(const SimulationEnvironment &env)
{
    timedLoop.ChangePeriod(static_cast<int>(env.deltaT * 1000.f));
    simulation.SetSimulationEnv(env);
}

