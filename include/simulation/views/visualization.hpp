#pragma once

#include "visualization/camera.hpp"
#include "visualization/grid.hpp"
#include "visualization/renderObject.hpp"
#include "visualization/shaders/stdShader.hpp"
#include "visualization/shaders/phongShader.hpp"
#include "visualization/shaders/bezierSurfaceShader.hpp"
#include "visualization/framebuffer.hpp"
#include "visualization/steeringCube.hpp"

#include "../model/simulationEnvironment.hpp"
#include "../model/springGraph.hpp"

#include "../utils/vector3D.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


class Visualization {
public:
    Visualization(int xResolution, int yResolution);

    void Render(const SpringGraph& springGraph, const Vector3D<MaterialPointId>& bezierPointsIds);

    void RotateCamera(float x, float y);

    [[nodiscard]]
    glm::vec3 GetCameraPosition() const
        { return camera.GetPosition(); }

    void SetCameraPosition(const glm::vec3& position)
        { camera.SetPosition(position); }

    void SetSimulationProperties(const SimulationEnvironment& simProperties);

    [[nodiscard]]
    SteeringCube& GetSteeringCube()
        { return steeringCube; }

    [[nodiscard]]
    const SteeringCube& GetSteeringCube() const
        { return steeringCube; }

    [[nodiscard]]
    bool IsMouseOverWindow() const
        { return mouseIsOver; }

    [[nodiscard]]
    static const char* WindowName()
        { return "Visualization"; }

private:
    bool mouseIsOver = false;

    Camera camera;
    Grid grid;

    Framebuffer framebuffer;

    RenderObject simulationArea;
    SteeringCube steeringCube;
    RenderObject sphere;
    Mesh softBody;
    Mesh springs;

    SimulationEnvironment properties;

    StdShader shader;
    PhongShader phongShader;
    BicubicBezierSurfaceShader bezierSurfaceShader;

    void UpdateSprings(const SpringGraph& springGraph);
    void UpdateSoftBody(const SpringGraph& springGraph, const Vector3D<MaterialPointId>& bezierPointsIds);
};
