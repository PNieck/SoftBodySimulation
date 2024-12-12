#pragma once

#include "mesh.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec4.hpp"

#include "shaders/stdShader.hpp"


class RenderObject {
public:
    RenderObject();

    [[nodiscard]]
    glm::mat4 ModelMatrix() const;

    void Rotate(const glm::mat4& mat)
        { rotationMatrix *= mat; }

    void SetRotation(const glm::mat4& mat)
        { rotationMatrix = mat; }

    void SetPosition(const glm::vec3& position)
        { translation = position; }

    void SetColor(const glm::vec4& newColor)
        { color = newColor; }

    void SetScale(const glm::vec3& newScale)
        { scale = newScale; }

    void SetScale(const float newScale)
        { scale = glm::vec3(newScale); }

    void UpdateMesh(
        const std::vector<float>& vertices,
        const std::vector<uint32_t>& indices,
        const Mesh::Type meshType
    )
        { mesh.Update(vertices, indices, meshType); }

    [[nodiscard]]
    const Mesh& GetMesh() const
        { return mesh; }

    Mesh& GetMesh()
        { return mesh; }

    void UseMesh() const
        { mesh.Use(); }

    void Render(const StdShader& shader, const glm::mat4& cameraMtx) const;

    [[nodiscard]]
    int MeshElements() const
        { return mesh.GetElementsCnt(); }

private:
    Mesh mesh;

    glm::vec4 color;
    glm::vec3 scale;
    glm::vec3 translation;
    glm::mat4 rotationMatrix;
};
