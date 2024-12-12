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

    [[nodiscard]]
    const glm::vec3& GetPosition() const
        { return RenderObject::GetPosition(); }

    [[nodiscard]]
    const glm::mat4& RotationMatrix() const
        { return RenderObject::RotationMatrix(); }

    void Render(const StdShader& shader, const glm::mat4& cameraMtx) const
        { RenderObject::Render(shader, cameraMtx); }
};
