#pragma once

#include "indexedMesh.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec4.hpp"


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

    void UpdateMesh(
        const std::vector<float>& vertices,
        const std::vector<uint32_t>& indices
    );

    void UseMesh() const
        { mesh.Use(); }

    [[nodiscard]]
    int MeshElements() const
        { return mesh.GetElementsCnt(); }

private:
    IndexedMesh mesh;
    glm::vec4 color;
    glm::vec3 scale;
    glm::vec3 translation;
    glm::mat4 rotationMatrix;
};
