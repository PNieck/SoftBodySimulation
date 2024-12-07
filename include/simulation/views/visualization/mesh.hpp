#pragma once

#include <vector>

#include <glad/glad.h>


class Mesh {
public:
    Mesh();
    Mesh(Mesh&& mesh) = default;
    Mesh(Mesh& mesh) = delete;
    ~Mesh();

    void Update(const std::vector<float>& vertices, const std::vector<uint32_t>& indices);

    [[nodiscard]]
    int GetElementsCnt() const
    { return elementsCnt; }

    void Use() const
    { glBindVertexArray(VAO); }

    Mesh& operator=(Mesh& mesh) = delete;

private:
    /// @brief OpenGl vector array object
    unsigned int VAO;

    /// @brief OpenGl vector buffer object
    unsigned int VBO;

    /// @brief OpenGl element buffer object
    unsigned int EBO;

    int elementsCnt;
};
