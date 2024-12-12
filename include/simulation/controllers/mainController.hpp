#pragma once

#include "mouseState.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../views/dockingSpace.hpp"
#include "../views/optionsPanel.hpp"
#include "../views/visualization.hpp"

#include "../model/springSimulator.hpp"

#include "../utils/vector3D.hpp"


class MainController {
public:
    explicit MainController(GLFWwindow* window);
    ~MainController();

    void StartSimulation()
        {  }

    void StopSimulation()
        {  }

    void UpdateSimulation()
        {  }

    [[nodiscard]]
    bool SimulationIsRunning() const
        { return true; }

    void Render();

    void MouseClicked(const MouseButton button)
        { mouseState.ButtonClicked(button); }

    void MouseReleased(const MouseButton button)
        { mouseState.ButtonReleased(button); }

    void MouseMoved(int x, int y);
    void ScrollMoved(int offset);

private:
    using SpringId = SpringGraph::SpringId;
    using MaterialPointId = SpringGraph::MaterialPointId;

    static constexpr float initialMaterialPointMass = 1.f;
    static constexpr float steeringCubeEdgeLen = 0.2f;

    MouseState mouseState;

    DockingSpace dockingSpace;
    OptionsPanel optionsPanel;
    Visualization visualization;

    Vector3D<MaterialPointId> bezierPointsIds;
    Vector3D<MaterialPointId> steeringPointsIds;
    SpringSimulator simulator;


    [[nodiscard]]
    bool WantToCaptureMouse() const;

    SpringGraph InitialSpringGraph();
};
