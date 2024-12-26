#include <simulation/views/visualization/grid.hpp>


Grid::Grid()
{
    // Vertices of a square on XY plane
    const std::vector<PositionVertex> vertices = {
        {  1.0f,  1.0f, 0.0f },
        {  1.0f, -1.0f, 0.0f },
        { -1.0f, -1.0f, 0.0f },
        { -1.0f,  1.0f, 0.0f }
   };

    const std::vector<uint32_t> indices = {
        0, 1, 3, // First triangle
        1, 2, 3  // Second triangle
    };

    //gridMesh.Use();
    gridMesh.Update<PositionVertex>(vertices, indices, Mesh::Triangles, Mesh::Usage::Static);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Grid::Render(const glm::mat4& view, const glm::mat4& projection, const float nearPlane, const float farPlane) const
{
    shader.Use();

    shader.SetNearPlane(nearPlane);
    shader.SetFarPlane(farPlane);
    shader.SetViewMatrix(view);
    shader.SetProjectionMatrix(projection);

    gridMesh.Use();
    glDrawElements(GL_TRIANGLES, gridMesh.GetElementsCnt(), GL_UNSIGNED_INT, 0);
}
