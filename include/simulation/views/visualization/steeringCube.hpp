#pragma once

#include "renderObject.hpp"
#include "meshFactory.hpp"


class SteeringCube : RenderObject {
public:
    SteeringCube() {
        MeshFactory::CubeWireframe<PositionVertex>(GetMesh());
    }

    [[nodiscard]]
    float GetEdgeLength() const
        { return GetScale().x; }

    void SetEdgeLength(const float length)
        { return SetScale(length); }

    void SetPosition(const glm::vec3& newPosition)
        { RenderObject::SetPosition(newPosition); }

    void SetRotation(const glm::quat& newRotation)
        { RenderObject::SetRotation(newRotation); }

    [[nodiscard]]
    const glm::vec3& GetPosition() const
        { return RenderObject::GetPosition(); }

    [[nodiscard]]
    glm::quat RotationQuat() const
        { return RenderObject::RotationQuat(); }

    void Render(const StdShader& shader, const glm::mat4& cameraMtx) const
        { RenderObject::Render(shader, cameraMtx); }
};
