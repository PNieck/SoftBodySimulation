#include "simulation/views/visualization.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <numbers>

#include <simulation/views/visualization/meshLoader.hpp>

#include "imgui.h"


Visualization::Visualization(const int xResolution, const int yResolution):
    camera({
        .target = glm::vec3(0.f),
        .viewportWidth = xResolution,
        .viewportHeight = yResolution,
        .fov = std::numbers::pi_v<float> / 4.f,
        .nearPlane = 0.5f,
        .farPlane = 100.0f,
    }),
    framebuffer(xResolution, yResolution)
{
    glViewport(0, 0, xResolution, yResolution);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(std::numeric_limits<uint32_t>::max());

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    UpdateSimulationAreaMesh();
}


void Visualization::Update(const glm::quat& q)
{
}


void Visualization::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    const auto view = camera.ViewMatrix();
    const auto projection = camera.ProjectionMatrix();

    const auto cameraMtx = projection * view;

    ImGui::Begin(WindowName());

    mouseIsOver = ImGui::IsWindowHovered();

    framebuffer.Use();

    glClear(GL_COLOR_BUFFER_BIT);
    grid.Render(view, projection);

    shader.Use();
    shader.SetColor(glm::vec4(0.969f, 0.957f, 0.212f, 1.f));
    shader.SetMVP(cameraMtx);
    simulationArea.Use();
    glDrawElements(GL_LINE_STRIP, simulationArea.GetElementsCnt(), GL_UNSIGNED_INT, nullptr);

    Framebuffer::UseDefault();

    const uint64_t textureID = framebuffer.GetColorTextureId();
    const ImVec2 size = ImGui::GetContentRegionAvail();
    ImGui::Image(textureID, ImVec2{ size.x, size.y });
    ImGui::End();
}


void Visualization::RotateCamera(const float x, const float y)
{
    const auto oldPos = camera.GetPosition();

    auto rotation = glm::mat4(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f
    );

    rotation = glm::rotate(rotation, x, glm::vec3(0.f, 1.f, 0.f));

    const auto axis = glm::cross(oldPos, glm::vec3(0.f, 1.f, 0.f));
    rotation = glm::rotate(rotation, y, axis);

    const auto newPos = rotation * glm::vec4(oldPos, 1.f);
    camera.SetPosition(glm::vec3(newPos.x, newPos.y, newPos.z));
}


void Visualization::SetSimulationProperties(const SimulationProperties &simProperties) {
    properties = simProperties;


}


void Visualization::UpdateSimulationAreaMesh() {
    const float coord = properties.simulationAreaEdgeLength / 2.f;

    const std::vector<float> vertices = {
        // Upper vertices
         coord, coord,  coord,
        -coord, coord,  coord,
        -coord, coord, -coord,
         coord, coord, -coord,

        // Lower vertices
         coord, -coord,  coord,
        -coord, -coord,  coord,
        -coord, -coord, -coord,
         coord, -coord, -coord
    };

    constexpr uint32_t primitiveRestart = std::numeric_limits<uint32_t>::max();

    const std::vector<uint32_t> indices = {
        0, 1, 2, 3, 0, primitiveRestart,  // Upper part
        4, 5, 6, 7, 4, primitiveRestart, // Lower part
        0, 4, primitiveRestart,
        1, 5, primitiveRestart,
        2, 6, primitiveRestart,
        3, 7, primitiveRestart
    };

    simulationArea.Update(vertices, indices);
}

