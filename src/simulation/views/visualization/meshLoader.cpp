#include <stdexcept>
#include <simulation/views/visualization/meshLoader.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


std::tuple<std::vector<float>, std::vector<uint32_t>> MeshLoader::LoadWithNormals(const std::string &filepath)
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

        // vertices.push_back(assimpMesh->mNormals[i].x);
        // vertices.push_back(assimpMesh->mNormals[i].y);
        // vertices.push_back(assimpMesh->mNormals[i].z);
    }

    std::vector<uint32_t> indices;
    indices.reserve(assimpMesh->mNumFaces * 3);

    for (int i=0; i < assimpMesh->mNumFaces; i++) {
        const aiFace face = assimpMesh->mFaces[i];
        for (int j=0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return {vertices, indices};
}
