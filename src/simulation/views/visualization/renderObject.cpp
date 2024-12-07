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


void RenderObject::Render(const StdShader &shader, const glm::mat4& cameraMtx) const {
    shader.SetColor(color);
    shader.SetMVP(cameraMtx * ModelMatrix());
    UseMesh();
    glDrawElements(mesh.GetType(), mesh.GetElementsCnt(), GL_UNSIGNED_INT, nullptr);
}
