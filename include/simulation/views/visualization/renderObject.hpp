#pragma once

#include "mesh.hpp"
#include "vertex.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/quaternion.hpp"

#include "shaders/stdShader.hpp"
#include "shaders/phongShader.hpp"
#include "shaders/bezierCubeShader.hpp"


class RenderObject {
public:
    RenderObject();

    [[nodiscard]]
    glm::mat4 ModelMatrix() const;

    [[nodiscard]]
    glm::mat4 ModelMatrixInverse() const;

    void Rotate(const glm::quat& quat)
        { rotationQuat *= quat; }

    void SetRotation(const glm::quat& quat)
        { rotationQuat = quat; }

    [[nodiscard]]
    glm::mat4 RotationMatrix() const
        { return mat4_cast(rotationQuat); }

    [[nodiscard]]
    glm::quat RotationQuat() const
        { return rotationQuat; }

    void SetPosition(const glm::vec3& position)
        { translation = position; }

    [[nodiscard]]
    const glm::vec3& GetPosition() const
        { return translation; }

    void SetColor(const glm::vec4& newColor)
        { color = newColor; }

    void SetColor(const float r, const float g, const float b, const float a = 1.f)
        { SetColor(glm::vec4(r, g, b, a)); }

    [[nodiscard]]
    const glm::vec4& GetColor() const
        { return color; }

    [[nodiscard]]
    const glm::vec3& GetScale() const
        { return scale; }

    void SetScale(const glm::vec3& newScale)
        { scale = newScale; }

    void SetScale(const float newScale)
        { scale = glm::vec3(newScale); }

    template <Vertex v>
    void UpdateMesh(
        const std::vector<v>& vertices,
        const std::vector<uint32_t>& indices,
        const Mesh::Type meshType,
        const Mesh::Usage usage
    )
        { mesh.Update(vertices, indices, meshType, usage); }

    [[nodiscard]]
    const Mesh& GetMesh() const
        { return mesh; }

    Mesh& GetMesh()
        { return mesh; }

    void UseMesh() const
        { mesh.Use(); }

    void Render(const StdShader& shader, const glm::mat4& cameraMtx) const;

    void Render(const PhongShader& shader) const;

    void Render(const BezierCubeShader& shader) const;

    [[nodiscard]]
    int MeshElements() const
        { return mesh.GetElementsCnt(); }

private:
    Mesh mesh;

    glm::vec4 color;
    glm::vec3 scale;
    glm::vec3 translation;
    glm::quat rotationQuat;
};
