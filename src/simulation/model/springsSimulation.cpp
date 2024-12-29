#include "simulation/model/springsSimulation.hpp"

#include <glm/glm.hpp>


void SpringsSimulation::UpdateEuler()
{
    std::lock_guard guard(simulationMutex);

    std::fill(v.begin(), v.end(), glm::vec3(0.f));
    std::fill(p.begin(), p.end(), glm::vec3(0.f));

    for (int i=0; i < springGraph.GetNeighbours().size(); i++) {
        auto& mp1 = springGraph.GetMaterialPoint(i);

        if (mp1.mass == std::numeric_limits<float>::infinity())
            continue;

        glm::vec3 forceAcc(0.f, 0.f, 0.f);

        for (const auto& neighbour: springGraph.GetNeighbours()[i]) {
            const auto& mp2 = springGraph.GetMaterialPoint(neighbour.materialPoint);
            const auto& spring = springGraph.GetSpring(neighbour.spring);

            forceAcc += SpringForce(mp1.position, mp2.position, mp1.velocity, mp2.velocity, spring.springCoef, spring.restLength);
        }

        p[i] += environment.deltaT * mp1.velocity;
        v[i] += environment.deltaT * forceAcc / mp1.mass;
    }

    for (int i=0; i < springGraph.GetNeighbours().size(); i++) {
        auto& mp1 = springGraph.GetMaterialPoint(i);
        mp1.position += p[i];
        mp1.velocity += v[i];
    }
}

void SpringsSimulation::UpdateRungeKutta() {
    std::lock_guard guard(simulationMutex);

    std::fill(v.begin(), v.end(), glm::vec3(0.f));
    std::fill(p.begin(), p.end(), glm::vec3(0.f));

    for (int i=0; i < springGraph.GetNeighbours().size(); i++) {
        auto& mp1 = springGraph.GetMaterialPoint(i);

        if (mp1.mass == std::numeric_limits<float>::infinity())
            continue;

        for (const auto& neighbour: springGraph.GetNeighbours()[i]) {
            const auto& mp2 = springGraph.GetMaterialPoint(neighbour.materialPoint);
            const auto& spring = springGraph.GetSpring(neighbour.spring);

            glm::vec3 k1_v1 = SpringForce(mp1.position, mp2.position, mp1.velocity, mp2.velocity, spring.springCoef, spring.restLength);
            glm::vec3 k1_v2 = -k1_v1;

            k1_v1 /= mp1.mass;
            k1_v2 /= mp2.mass;

            glm::vec3 k1_p1 = mp1.velocity;
            glm::vec3 k1_p2 = mp2.velocity;

            glm::vec3 k2_v1 = SpringForce(
                mp1.position + environment.deltaT/2.f * k1_p1,
                mp2.position + environment.deltaT/2.f * k1_p2,
                mp1.velocity + environment.deltaT/2.f * k1_v1,
                mp2.velocity + environment.deltaT/2.f * k1_v2,
                spring.springCoef, spring.restLength
            );
            glm::vec3 k2_v2 = -k2_v1;

            k2_v1 /= mp1.mass;
            k2_v2 /= mp2.mass;

            glm::vec3 k2_p1 = mp1.velocity + environment.deltaT/2.f * k2_v1;
            glm::vec3 k2_p2 = mp2.velocity + environment.deltaT/2.f * k2_v2;

            glm::vec3 k3_v1 = SpringForce(
                mp1.position + environment.deltaT/2.f * k2_p1,
                mp2.position + environment.deltaT/2.f * k2_p2,
                mp1.velocity + environment.deltaT/2.f * k2_v1,
                mp2.velocity + environment.deltaT/2.f * k2_v2,
                spring.springCoef, spring.restLength
            );
            glm::vec3 k3_v2 = -k3_v1;

            k3_v1 /= mp1.mass;
            k3_v2 /= mp2.mass;

            glm::vec3 k3_p1 = mp1.velocity + environment.deltaT/2.f * k3_v1;
            glm::vec3 k3_p2 = mp2.velocity + environment.deltaT/2.f * k3_v2;

            glm::vec3 k4_v1 = SpringForce(
                mp1.position + environment.deltaT * k3_p1,
                mp2.position + environment.deltaT * k3_p2,
                mp1.velocity + environment.deltaT * k3_v1,
                mp2.velocity + environment.deltaT * k3_v2,
                spring.springCoef, spring.restLength
            );

            glm::vec3 k4_v2 = -k4_v1;

            k4_v1 /= mp1.mass;
            k4_v2 /= mp2.mass;

            glm::vec3 k4_p1 = mp1.velocity + environment.deltaT * k4_v1;
            //glm::vec3 k4_p2 = mp2.velocity + environment.deltaT * k4_v2;

            v[i] += environment.deltaT/6.f * (k1_v1 + 2.f*k2_v1 + 2.f*k3_v1 + k4_v1);
            p[i] += environment.deltaT/6.f * (k1_p1 + 2.f*k2_p1 + 2.f*k3_p1 + k4_p1);
        }
    }

    for (int i=0; i < springGraph.GetNeighbours().size(); i++) {
        auto& mp1 = springGraph.GetMaterialPoint(i);
        mp1.position += p[i];
        mp1.velocity += v[i];
    }
}


glm::vec3 SpringsSimulation::SpringForce(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &v1,
    const glm::vec3 &v2, const float springCoef, const float springLen)
{
    if (p1 == p2)
        return glm::vec3(0.f);

    const float dotI = DotI(p1, p2, v1, v2);
    const float springExtension = length(p1 - p2) - springLen;

    const float forceVal = -environment.dampingCoef * dotI - springCoef * springExtension;
    return normalize(p1 - p2) * forceVal;
}


float SpringsSimulation::DotI(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &v1, const glm::vec3 &v2)
{
    const float len = glm::length(p1 - p2);
    if (len == 0.f)
        return 0.f;

    return glm::dot(p1 - p2, v1 - v2) / len;
}

