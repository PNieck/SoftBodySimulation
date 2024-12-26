#include "simulation/views/visualization.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <numbers>

#include <simulation/views/visualization/meshFactory.hpp>

#include "imgui.h"


Visualization::Visualization(const int xResolution, const int yResolution):
    camera({
        .target = glm::vec3(0.f),
        .viewportWidth = xResolution,
        .viewportHeight = yResolution,
        .fov = std::numbers::pi_v<float> / 4.f,
        .nearPlane = 0.1f,
        .farPlane = 100.0f,
    }),
    framebuffer(xResolution, yResolution)
{
    glViewport(0, 0, xResolution, yResolution);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(std::numeric_limits<uint32_t>::max());

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);

    MeshFactory::CubeWireframe<PositionVertex>(simulationArea.GetMesh());
    MeshFactory::LoadFromFile<PosNormalVertex>(sphere.GetMesh(), "../../models/icosphere.obj");
    sphere.SetScale(0.01f);
}


void Visualization::Render(const SpringGraph& springGraph, const Vector3D<MaterialPointId>& bezierPointsIds)
{
    ImGui::Begin(WindowName());

    mouseIsOver = ImGui::IsWindowHovered();

    framebuffer.Use();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.Use();

    const auto view = camera.ViewMatrix();
    const auto projection = camera.ProjectionMatrix();

    const auto cameraMtx = projection * view;
    simulationArea.Render(shader, cameraMtx);
    steeringCube.Render(shader, cameraMtx);

    phongShader.Use();
    phongShader.SetCameraPosition(camera.GetPosition());
    phongShader.SetViewMatrix(view);
    phongShader.SetProjectionMatrix(projection);
    phongShader.SetLightPosition(glm::vec3(10.0f, 10.f, 10.f));

    for (const auto& point: springGraph.MaterialPoints()) {
        sphere.SetPosition(point.position);
        sphere.Render(phongShader);
    }

    // UpdateSprings(springGraph);
    // shader.Use();
    // shader.SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));     // White
    // shader.SetMVP(cameraMtx);
    // springs.Use();
    // glDrawElements(springs.GetType(), springs.GetElementsCnt(), GL_UNSIGNED_INT, nullptr);

    UpdateSoftBody(springGraph, bezierPointsIds);
    bezierSurfaceShader.Use();
    bezierSurfaceShader.SetColor(glm::vec4(0.145f, 0.604f, 0.831f, 1.f));
    bezierSurfaceShader.SetProjection(projection);
    bezierSurfaceShader.SetModelMatrix(glm::mat4(1.f));
    bezierSurfaceShader.SetViewMatrix(view);
    bezierSurfaceShader.SetLightPosition(glm::vec3(10.0f, 10.f, 10.f));
    bezierSurfaceShader.SetCameraPosition(camera.GetPosition());

    softBody.Use();
    constexpr float v[] = {64.f, 64.f, 64.f, 64.f};
    glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, v);
    glPatchParameteri(GL_PATCH_VERTICES, 16);
    glDrawElements(softBody.GetType(), softBody.GetElementsCnt(), GL_UNSIGNED_INT, nullptr);

    // normalsCheckShader.Use();
    // normalsCheckShader.SetProjection(projection);
    // normalsCheckShader.SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    // normalsCheckShader.SetViewMatrix(view);
    // normalsCheckShader.SetModelMatrix(glm::mat4(1.0f));

    // softBody.Use();
    // glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, v);
    // glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, v);
    // glPatchParameteri(GL_PATCH_VERTICES, 16);
    // glDrawElements(softBody.GetType(), softBody.GetElementsCnt(), GL_UNSIGNED_INT, nullptr);

    const auto camParams = camera.GetParameters();
    grid.Render(view, projection, camParams.nearPlane, camParams.farPlane);

    Framebuffer::UseDefault();

    const uint64_t textureID = framebuffer.GetColorTextureId();
    const ImVec2 size = ImGui::GetContentRegionAvail();
    ImGui::Image(textureID, ImVec2{ size.x, size.y }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
    ImGui::End();
}


void Visualization::RotateCamera(const float x, const float y)
{
    const auto oldPos = camera.GetPosition();

    auto rotation = rotate(glm::mat4(1.f), x, glm::vec3(0.f, 1.f, 0.f));

    const auto axis = cross(oldPos, glm::vec3(0.f, 1.f, 0.f));
    rotation = rotate(rotation, y, axis);

    const auto newPos = rotation * glm::vec4(oldPos, 1.f);
    camera.SetPosition(glm::vec3(newPos.x, newPos.y, newPos.z));
}


void Visualization::SetSimulationProperties(const SimulationEnvironment &simProperties) {
    properties = simProperties;

    simulationArea.SetScale(simProperties.simulationAreaEdgeLength);
}


void Visualization::UpdateSprings(const SpringGraph &springGraph) {
    std::vector<PositionVertex> vertices;
    std::vector<uint32_t> indices;
    int i=0;

    vertices.reserve(springGraph.Springs().size() * 2);
    indices.reserve(springGraph.Springs().size() * 2);

    for (const auto& spring: springGraph.Springs()) {
        vertices.emplace_back(springGraph.GetMaterialPoint(spring.anchorPoint1).position);
        vertices.emplace_back(springGraph.GetMaterialPoint(spring.anchorPoint2).position);

        indices.push_back(i++);
        indices.push_back(i++);
    }

    springs.Update(vertices, indices, Mesh::Lines, Mesh::Usage::Dynamic);
}


void Visualization::UpdateSoftBody(const SpringGraph& springGraph, const Vector3D<MaterialPointId>& bezierPointsIds) {
    std::vector<PositionVertex> vertices;
    std::vector<uint32_t> indices;
    int i=0;

    vertices.reserve(16 * 6);
    indices.reserve(16 * 6);

    for (size_t x=0; x < bezierPointsIds.SizeX(); x++) {
        for (size_t y=0; y < bezierPointsIds.SizeY(); y++) {
            const MaterialPointId id = bezierPointsIds.At(x, y, 0);

            vertices.emplace_back(springGraph.GetMaterialPoint(id).position);
            indices.push_back(i++);
        }
    }

    for (size_t x=0; x < bezierPointsIds.SizeX(); x++) {
        for (size_t y=0; y < bezierPointsIds.SizeY(); y++) {
            const MaterialPointId id = bezierPointsIds.At(
                x,
                bezierPointsIds.SizeY() - y - 1,
                bezierPointsIds.SizeZ() - 1
            );

            vertices.emplace_back(springGraph.GetMaterialPoint(id).position);
            indices.push_back(i++);
        }
    }

    for (size_t x=0; x < bezierPointsIds.SizeX(); x++) {
        for (size_t z=0; z < bezierPointsIds.SizeZ(); z++) {
            const MaterialPointId id = bezierPointsIds.At(
                x,
                bezierPointsIds.SizeY() - 1,
                z
            );

            vertices.emplace_back(springGraph.GetMaterialPoint(id).position);
            indices.push_back(i++);
        }
    }

    for (size_t x=0; x < bezierPointsIds.SizeX(); x++) {
        for (size_t z=0; z < bezierPointsIds.SizeZ(); z++) {
            const MaterialPointId id = bezierPointsIds.At(x, 0, bezierPointsIds.SizeZ() - z - 1);

            vertices.emplace_back(springGraph.GetMaterialPoint(id).position);
            indices.push_back(i++);
        }
    }

    for (size_t y=0; y < bezierPointsIds.SizeY(); y++) {
        for (size_t z=0; z < bezierPointsIds.SizeZ(); z++) {
            const MaterialPointId id = bezierPointsIds.At(0, y, z);

            vertices.emplace_back(springGraph.GetMaterialPoint(id).position);
            indices.push_back(i++);
        }
    }

    for (size_t y=0; y < bezierPointsIds.SizeY(); y++) {
        for (size_t z=0; z < bezierPointsIds.SizeZ(); z++) {
            const MaterialPointId id = bezierPointsIds.At(
                bezierPointsIds.SizeX()-1,
                bezierPointsIds.SizeY() - y - 1,
                z
            );

            vertices.emplace_back(springGraph.GetMaterialPoint(id).position);
            indices.push_back(i++);
        }
    }

    softBody.Update(vertices, indices, Mesh::Type::Patches, Mesh::Usage::Dynamic);
}

