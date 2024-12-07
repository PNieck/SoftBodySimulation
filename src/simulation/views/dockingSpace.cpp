#include <simulation/views/dockingSpace.hpp>

#include "imgui_internal.h"

#include "simulation/views/visualization.hpp"
#include "simulation/views/optionsPanel.hpp"


DockingSpace::DockingSpace()
{
    windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground;

    dockNodeFlags = ImGuiDockNodeFlags_PassthruCentralNode;
}


void DockingSpace::Render() const
{
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace", nullptr, windowFlags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockSpaceId = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockNodeFlags);

        static auto first_time = true;
        if (first_time)
        {
            first_time = false;

            ImGui::DockBuilderRemoveNode(dockSpaceId); // clear any previous layout
            ImGui::DockBuilderAddNode(dockSpaceId, dockNodeFlags | ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockSpaceId, viewport->Size);

            const auto rightDockId = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Right, 0.25f, nullptr, &dockSpaceId);

            ImGui::DockBuilderDockWindow(OptionsPanel::WindowName(), rightDockId);
            ImGui::DockBuilderDockWindow(Visualization::WindowName(), dockSpaceId);

            ImGui::DockBuilderFinish(dockSpaceId);
        }
    }

    ImGui::End();
}
