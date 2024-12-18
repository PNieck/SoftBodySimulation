#pragma once

#include <vector>

#include <glad/glad.h>

#include "vertex.hpp"


class Mesh {
public:
    enum Type {
        Triangles = GL_TRIANGLES,
        Lines = GL_LINES,
        LinesStrip = GL_LINE_STRIP,
        Patches = GL_PATCHES
    };

    enum Usage {
        Static = GL_STATIC_DRAW,
        Dynamic = GL_DYNAMIC_DRAW,
    };

    Mesh();
    Mesh(Mesh&& mesh) = default;
    Mesh(Mesh& mesh) = delete;
    ~Mesh();

    template <Vertex v, class Allocator>
    void Update(
        const std::vector<v, Allocator>& vertices,
        const std::vector<uint32_t>& indices,
        const Type meshType,
        const Usage usage
    ) {
        Use();

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(v) * vertices.size(), vertices.data(), usage);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), usage);

        v::SetupAttributes();

        type = meshType;
        elementsCnt = indices.size();
    }

    [[nodiscard]]
    int GetElementsCnt() const
        { return elementsCnt; }

    void Use() const
        { glBindVertexArray(VAO); }

    [[nodiscard]]
    Type GetType() const
        { return type; }

    Mesh& operator=(Mesh& mesh) = delete;

private:
    /// @brief OpenGl vector array object
    unsigned int VAO = 0;

    /// @brief OpenGl vector buffer object
    unsigned int VBO = 0;

    /// @brief OpenGl element buffer object
    unsigned int EBO = 0;

    Type type = Triangles;
    int elementsCnt = 0;
};
