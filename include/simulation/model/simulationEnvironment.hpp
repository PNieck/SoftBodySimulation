#pragma once


class SimulationEnvironment
{
public:
    float deltaT = 0.01f;
    float simulationAreaEdgeLength = 2.f;
    float springDamping = 4.f;
    float viscousDamping = 0.1f;
    float collisionDamping = 0.8f;
};
