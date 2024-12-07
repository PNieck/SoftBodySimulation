#pragma once

#include <string>

#include "mesh.hpp"


class MeshFactory {
public:
    static void LoadFromFile(Mesh& mesh, const std::string& filepath);

    static void CubeWireframe(Mesh& mesh, float edge = 1.f);
};
