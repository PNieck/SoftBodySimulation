#include <stdexcept>
#include <simulation/views/visualization/meshFactory.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


constexpr auto defaultReadFlags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;


template <>
void MeshFactory::LoadFromFile<PositionVertex>(Mesh& mesh, const std::string &filepath, const Mesh::Usage usage)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filepath, defaultReadFlags);
    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error("Failed to load model");
    }

    const aiMesh* assimpMesh = scene->mMeshes[0];

    std::vector<PositionVertex> vertices;
    vertices.reserve(assimpMesh->mNumVertices);

    for (int i=0; i < assimpMesh->mNumVertices; i++) {
        vertices.emplace_back(
            assimpMesh->mVertices[i].x,
            assimpMesh->mVertices[i].y,
            assimpMesh->mVertices[i].z
        );
    }

    mesh.Update(vertices, ParseIndices(assimpMesh), Mesh::Type::Triangles, usage);
}


template <>
void MeshFactory::LoadFromFile<PosNormalVertex>(Mesh& mesh, const std::string &filepath, const Mesh::Usage usage)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filepath, defaultReadFlags | aiProcess_GenSmoothNormals);
    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error("Failed to load model");
    }

    const aiMesh* assimpMesh = scene->mMeshes[0];

    std::vector<PosNormalVertex> vertices;
    vertices.reserve(assimpMesh->mNumVertices);

    for (int i=0; i < assimpMesh->mNumVertices; i++) {
        glm::vec3 position(
            assimpMesh->mVertices[i].x,
            assimpMesh->mVertices[i].y,
            assimpMesh->mVertices[i].z
        );

        glm::vec3 normal(
            assimpMesh->mNormals[i].x,
            assimpMesh->mNormals[i].y,
            assimpMesh->mNormals[i].z
        );

        vertices.emplace_back(position, normal);
    }

    mesh.Update(vertices, ParseIndices(assimpMesh), Mesh::Type::Triangles, usage);
}


void UpdateMinMaxVector(glm::vec3& min, glm::vec3& max, const glm::vec3& vec)
{
    if (max.x < vec.x)
        max.x = vec.x;
    if (min.x > vec.x)
        min.x = vec.x;

    if (max.y < vec.y)
        max.y = vec.y;
    if (min.y > vec.y)
        min.y = vec.y;

    if (max.z < vec.z)
        max.z = vec.z;
    if (min.z > vec.z)
        min.z = vec.z;
}


template <>
void MeshFactory::LoadFromFile<BezierCubeCoordinatesWithNormals>(Mesh& mesh, const std::string &filepath, const Mesh::Usage usage)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filepath, defaultReadFlags | aiProcess_GenSmoothNormals);
    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error("Failed to load model");
    }

    const aiMesh* assimpMesh = scene->mMeshes[0];

    std::vector<BezierCubeCoordinatesWithNormals> vertices;
    vertices.reserve(assimpMesh->mNumVertices);

    glm::vec3 maxCoordinates(-std::numeric_limits<float>::infinity());
    glm::vec3 minCoordinates(std::numeric_limits<float>::infinity());

    for (int i=0; i < assimpMesh->mNumVertices; i++) {
        glm::vec3 position(
            assimpMesh->mVertices[i].x,
            assimpMesh->mVertices[i].y,
            assimpMesh->mVertices[i].z
        );

        glm::vec3 normal(
            assimpMesh->mNormals[i].x,
            assimpMesh->mNormals[i].y,
            assimpMesh->mNormals[i].z
        );

        normal *= 0.1f;
        normal += position;

        vertices.emplace_back(position, normal);

        UpdateMinMaxVector(minCoordinates, maxCoordinates, position);
        UpdateMinMaxVector(minCoordinates, maxCoordinates, normal);
    }

    glm::vec3 difference = maxCoordinates - minCoordinates;
    std::ranges::for_each(vertices,
     [&minCoordinates, &difference](BezierCubeCoordinatesWithNormals& vertex) {
        vertex.position.x = (vertex.position.x - minCoordinates.x) / difference.x;
        vertex.position.y = (vertex.position.y - minCoordinates.y) / difference.y;
        vertex.position.z = (vertex.position.z - minCoordinates.z) / difference.z;

        vertex.normalPosition.x = (vertex.normalPosition.x - minCoordinates.x) / difference.x;
        vertex.normalPosition.y = (vertex.normalPosition.y - minCoordinates.y) / difference.y;
        vertex.normalPosition.z = (vertex.normalPosition.z - minCoordinates.z) / difference.z;
    });

    mesh.Update(vertices, ParseIndices(assimpMesh), Mesh::Type::Triangles, usage);
}


template <>
void MeshFactory::CubeWireframe<PositionVertex>(Mesh& mesh, const float edge, const Mesh::Usage usage) {
    const float halfEdge = edge / 2.f;

    const std::vector<PositionVertex> vertices = {
        // Upper vertices
        {  halfEdge, halfEdge,  halfEdge },
        { -halfEdge, halfEdge,  halfEdge },
        { -halfEdge, halfEdge, -halfEdge },
        {  halfEdge, halfEdge, -halfEdge },

       // Lower vertices
        {  halfEdge, -halfEdge,  halfEdge },
        { -halfEdge, -halfEdge,  halfEdge },
        { -halfEdge, -halfEdge, -halfEdge },
        {  halfEdge, -halfEdge, -halfEdge }
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

    mesh.Update(vertices, indices, Mesh::Type::LinesStrip, usage);
}


std::vector<uint32_t> MeshFactory::ParseIndices(const aiMesh *assimpMesh) {
    std::vector<uint32_t> indices;
    indices.reserve(assimpMesh->mNumFaces * 3);

    for (int i=0; i < assimpMesh->mNumFaces; i++) {
        const aiFace face = assimpMesh->mFaces[i];
        for (int j=0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return indices;
}
