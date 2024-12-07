#pragma once

#include <vector>

#include <glad/glad.h>


class IndexedMesh {
public:
    IndexedMesh();
    ~IndexedMesh();

    void Update(const std::vector<float>& vertices, const std::vector<uint32_t>& indices);

    [[nodiscard]]
    int GetElementsCnt() const
        { return elementsCnt; }

    void Use() const
        { glBindVertexArray(VAO); }

private:
    /// @brief OpenGl vector array object
    unsigned int VAO = 0;

    /// @brief OpenGl vector buffer object
    unsigned int VBO = 0;

    /// @brief OpenGl element buffer object
    unsigned int EBO = 0;

    int elementsCnt = 0;
};
