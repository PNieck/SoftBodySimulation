#pragma once

#include <string>

#include "mesh.hpp"

#include <assimp/scene.h>


class MeshFactory {
public:
    template <Vertex v>
    static void LoadFromFile(Mesh& mesh, const std::string& filepath);

    template <Vertex v>
    static void CubeWireframe(Mesh& mesh, float edge = 1.f);

private:
    static std::vector<uint32_t> ParseIndices(const aiMesh* assimpMesh);
};
