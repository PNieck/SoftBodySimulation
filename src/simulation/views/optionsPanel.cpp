#include <simulation/views/optionsPanel.hpp>

#include <imgui.h>

#include "simulation/controllers/mainController.hpp"


OptionsPanel::OptionsPanel(MainController &controller):
    controller(controller)
{
}


void OptionsPanel::Render()
{
    ImGui::Begin(WindowName());

    const bool simRuns = controller.SimulationIsRunning();

    ImGui::PushItemWidth(-FLT_MIN);

    RenderStartStopButton(simRuns);
    RenderSteeringCubeOptions();

    ImGui::PopItemWidth();

    ImGui::End();
}


void OptionsPanel::RenderStartStopButton(const bool simRuns)
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

    if (ImGui::Button("Reset")) {
        //controller.SetProperties(properties);
    }
    ImGui::EndDisabled();
}


void OptionsPanel::RenderSteeringCubeOptions() const {
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
