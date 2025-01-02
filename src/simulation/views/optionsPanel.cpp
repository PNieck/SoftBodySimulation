#include <simulation/views/optionsPanel.hpp>

#include <imgui.h>

#include "simulation/controllers/mainController.hpp"


OptionsPanel::OptionsPanel(MainController &controller):
    controller(controller)
{
}


void OptionsPanel::Render() const
{
    ImGui::Begin(WindowName());

    ImGui::PushItemWidth(-FLT_MIN);

    const bool simRuns = controller.SimulationIsRunning();

    RenderStartStopButton(simRuns);
    RenderSteeringCubeOptions();
    RenderVisualizationOptions();
    RenderDisturbOptions();
    RenderSimulationOptions(simRuns);
    RenderSimulationInfo();

    ImGui::PopItemWidth();

    ImGui::End();
}


void OptionsPanel::RenderStartStopButton(const bool simRuns) const
{
    ImGui::BeginDisabled(simRuns);
    if (ImGui::Button("Start")) {
        controller.StartSimulation();
    }
    ImGui::EndDisabled();

    ImGui::SameLine();

    ImGui::BeginDisabled(!simRuns);
    if (ImGui::Button("Stop")) {
        controller.StopSimulation();
    }
    ImGui::EndDisabled();

    ImGui::BeginDisabled(simRuns);
    if (ImGui::Button("Update")) {
        controller.UpdateSimulation();
    }
    ImGui::EndDisabled();
}


void OptionsPanel::RenderSteeringCubeOptions() const
{
    ImGui::SeparatorText("Steering cube options");

    const auto& pos = controller.GetSteeringCubePosition();
    float coordinates[3] = {pos.x, pos.y, pos.z};

    ImGui::Text("Position");
    if (ImGui::DragFloat3("##Position", coordinates, 0.01f)) {
        controller.SetSteeringCubePosition(
            glm::vec3(coordinates[0], coordinates[1], coordinates[2])
        );
    }

    const auto euler = eulerAngles(controller.GetSteeringCubeRotationQuat());
    coordinates[0] = euler.x;
    coordinates[1] = euler.y;
    coordinates[2] = euler.z;

    ImGui::Text("Rotation (XYZ)");
    if (ImGui::DragFloat3("##SEulerAngles", coordinates, 0.01f)) {
        controller.SetSteeringCubeRotation(
            glm::vec3(coordinates[0], coordinates[1], coordinates[2])
        );
    }
}

void OptionsPanel::RenderVisualizationOptions() const
{
    ImGui::SeparatorText("Visualisation options");

    bool value = controller.GetRenderingSprings();
    if (ImGui::Checkbox("Render springs", &value))
        controller.SetRenderingSprings(value);

    value = controller.GetRenderingMassPoints();
    if (ImGui::Checkbox("Render mass points", &value))
        controller.SetRenderingMassPoints(value);

    value = controller.GetRenderingNormals();
    if (ImGui::Checkbox("Render cube normals", &value))
        controller.SetRenderingNormals(value);

    value = controller.GetRenderCube();
    if (ImGui::Checkbox("Render cube", &value))
        controller.SetRenderCube(value);

    value = controller.GetRenderMonkey();
    if (ImGui::Checkbox("Render monkey", &value))
        controller.SetRenderMonkey(value);
}


void OptionsPanel::RenderDisturbOptions() const
{
    ImGui::SeparatorText("Disturb soft body");

    static float maxDisturb = 5.f;
    ImGui::Text("Max disturb");
    ImGui::DragFloat("##maxDisturb", &maxDisturb, 0.01f);

    if (ImGui::Button("Disturb"))
        controller.DisturbSoftBody(maxDisturb);
}


void OptionsPanel::RenderSimulationOptions(const bool simRuns) const
{
    ImGui::SeparatorText("Simulation options");

    ImGui::BeginDisabled(simRuns);

    float value = controller.GetBezierSpringsCoefficient();
    ImGui::Text("Bezier cube springs coefficient");
    if (ImGui::DragFloat("##bezierSpringsCoef", &value, 1.0f, 0.01f, 100.f))
        controller.SetBezierSpringsCoefficient(value);

    value = controller.GetSteeringSpringsCoefficient();
    ImGui::Text("Steering cube springs coefficient");
    if (ImGui::DragFloat("##steeringSpringsCoef", &value, 1.0f, 0.01f, 100.f))
        controller.SetSteeringSpringsCoefficient(value);

    ImGui::EndDisabled();
}


void OptionsPanel::RenderSimulationInfo() {
    ImGui::SeparatorText("Simulation information");

    ImGui::Text("Visualization fps: %.2f", ImGui::GetIO().Framerate);
}
