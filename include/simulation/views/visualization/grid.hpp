#pragma once

#include "shaders/gridShader.hpp"
#include "indexedMesh.hpp"


class Grid {
public:
    Grid();

    void Render(const glm::mat4& view, const glm::mat4& projection) const;

private:
    GridShader shader;
    IndexedMesh gridMesh;
};
