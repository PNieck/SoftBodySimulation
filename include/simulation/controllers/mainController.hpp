#pragma once

#include "mouseState.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../views/dockingSpace.hpp"
#include "../views/optionsPanel.hpp"
#include "../views/visualization.hpp"

#include "../model/model.hpp"

#include "../utils/vector3D.hpp"


class MainController {
public:
    explicit MainController(GLFWwindow* window);
    ~MainController();

    void StartSimulation()
        { model.StartSimulation(); }

    void StopSimulation()
        { model.EndSimulation(); }

    void UpdateSimulation()
        { model.UpdateSimulation(); }

    [[nodiscard]]
    bool SimulationIsRunning() const
        { return model.IsSimulationRunning(); }

    void Render();

    void MouseClicked(const MouseButton button)
        { mouseState.ButtonClicked(button); }

    void MouseReleased(const MouseButton button)
        { mouseState.ButtonReleased(button); }

    void MouseMoved(int x, int y);
    void ScrollMoved(int offset);

    void SetSteeringCubePosition(const glm::vec3& position);

    [[nodiscard]]
    const glm::vec3& GetSteeringCubePosition() const
        { return visualization.GetSteeringCube().GetPosition(); }

    void SetSteeringCubeRotation(const glm::quat& rotation);

    [[nodiscard]]
    glm::quat GetSteeringCubeRotationQuat() const
        { return visualization.GetSteeringCube().RotationQuat(); }

private:
    static constexpr float initialMaterialPointMass = 1.f;
    static constexpr float steeringCubeEdgeLen = 0.2f;
    static constexpr float initialBezierSpringsCoef = 5.f;
    static constexpr float steeringSpringsCoef = 30.f;

    MouseState mouseState;

    DockingSpace dockingSpace;
    OptionsPanel optionsPanel;
    Visualization visualization;

    Vector3D<MaterialPointId> bezierPointsIds;
    Vector3D<MaterialPointId> steeringPointsIds;
    Model model;


    [[nodiscard]]
    bool WantToCaptureMouse() const
        { return !visualization.IsMouseOverWindow(); }

    SpringGraph InitialSpringGraph();
    void UpdateSteeringMaterialPoints();
};
