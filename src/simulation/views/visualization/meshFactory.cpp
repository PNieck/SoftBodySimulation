#include <stdexcept>
#include <simulation/views/visualization/meshFactory.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


void MeshFactory::LoadFromFile(Mesh& mesh, const std::string &filepath)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error("Failed to load model");
    }

    const aiMesh* assimpMesh = scene->mMeshes[0];

    std::vector<float> vertices;
    vertices.reserve(assimpMesh->mNumVertices * 6);

    for (int i=0; i < assimpMesh->mNumVertices; i++) {
        vertices.push_back(assimpMesh->mVertices[i].x);
        vertices.push_back(assimpMesh->mVertices[i].y);
        vertices.push_back(assimpMesh->mVertices[i].z);
    }

    std::vector<uint32_t> indices;
    indices.reserve(assimpMesh->mNumFaces * 3);

    for (int i=0; i < assimpMesh->mNumFaces; i++) {
        const aiFace face = assimpMesh->mFaces[i];
        for (int j=0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    mesh.Update(vertices, indices, Mesh::Type::Triangles);
}


void MeshFactory::CubeWireframe(Mesh& mesh, const float edge) {
    const float halfEdge = edge / 2.f;

    const std::vector<float> vertices = {
        // Upper vertices
        halfEdge, halfEdge,  halfEdge,
       -halfEdge, halfEdge,  halfEdge,
       -halfEdge, halfEdge, -halfEdge,
        halfEdge, halfEdge, -halfEdge,

       // Lower vertices
        halfEdge, -halfEdge,  halfEdge,
       -halfEdge, -halfEdge,  halfEdge,
       -halfEdge, -halfEdge, -halfEdge,
        halfEdge, -halfEdge, -halfEdge
   };

    constexpr uint32_t primitiveRestart = std::numeric_limits<uint32_t>::max();

    const std::vector<uint32_t> indices = {
        0, 1, 2, 3, 0, primitiveRestart,  // Upper part
        4, 5, 6, 7, 4, primitiveRestart, // Lower part
        0, 4, primitiveRestart,
        1, 5, primitiveRestart,
        2, 6, primitiveRestart,
        3, 7, primitiveRestart
    };

    mesh.Update(vertices, indices, Mesh::Type::LinesStrip);
}
