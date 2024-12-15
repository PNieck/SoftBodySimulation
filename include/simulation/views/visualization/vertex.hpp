#pragma once

#include <glm/vec3.hpp>


template <typename T>
concept Vertex = requires(T t) {
    { T::SetupAttributes() };
};


class PositionVertex {
public:
    PositionVertex(const glm::vec3& pos):
        position(pos) {}

    PositionVertex(const float x, const float y, const float z):
        position(x, y, z) {}

    glm::vec3 position;

    static void SetupAttributes() {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(0));
    }
};


class PosNormalVertex {
public:
    PosNormalVertex(const glm::vec3& pos, const glm::vec3& normal):
        position(pos), normal(normal) {}

    glm::vec3 position;
    glm::vec3 normal;

    static void SetupAttributes() {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float)*3));
        glEnableVertexAttribArray(1);
    }
};
