#include  <simulation/views/visualization/renderObject.hpp>

#include <glm/gtc/matrix_transform.hpp>


RenderObject::RenderObject():
    color(1.f),             // white color
    scale(1.f),
    translation(0.f),
    rotationMatrix(1.f)
{
}


glm::mat4 RenderObject::ModelMatrix() const
{
    auto result = glm::translate(glm::mat4(1.f), translation);
    result = glm::scale(result, scale);
    return  result * rotationMatrix;
}


void RenderObject::UpdateMesh(const std::vector<float> &vertices, const std::vector<uint32_t> &indices)
{
    mesh.Update(vertices, indices);
}
