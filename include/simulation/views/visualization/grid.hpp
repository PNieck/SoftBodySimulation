#pragma once

#include "shaders/gridShader.hpp"
#include "mesh.hpp"


class Grid {
public:
    Grid();

    void Render(const glm::mat4& view, const glm::mat4& projection, float nearPlane, float farPlane) const;

private:
    GridShader shader;
    Mesh gridMesh;
};
