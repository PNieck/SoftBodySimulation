#include  <simulation/views/visualization/renderObject.hpp>

#include <glm/gtc/matrix_transform.hpp>


RenderObject::RenderObject():
    color(1.f),             // white color
    scale(1.f),
    translation(0.f),
    rotationQuat(1.f, 0.f, 0.f, 0.f)
{
}


glm::mat4 RenderObject::ModelMatrix() const
{
    auto result = translate(glm::mat4(1.f), translation);
    result = glm::scale(result, scale);
    return  result * RotationMatrix();
}


glm::mat4 RenderObject::ModelMatrixInverse() const
{
    auto result = transpose(RotationMatrix());
    result = glm::scale(result, glm::vec3(1.f/scale.x, 1.f/scale.y, 1.f/scale.z));
    return translate(result, -translation);
}


void RenderObject::Render(const StdShader &shader, const glm::mat4& cameraMtx) const
{
    shader.SetColor(color);
    shader.SetMVP(cameraMtx * ModelMatrix());
    UseMesh();
    glDrawElements(mesh.GetType(), mesh.GetElementsCnt(), GL_UNSIGNED_INT, nullptr);
}


void RenderObject::Render(const PhongShader &shader) const
{
    shader.SetColor(color);
    shader.SetModelMatrix(ModelMatrix());
    shader.SetModelMatrixInverse(ModelMatrixInverse());

    UseMesh();

    glDrawElements(mesh.GetType(), mesh.GetElementsCnt(), GL_UNSIGNED_INT, nullptr);
}


void RenderObject::Render(const BezierCubeShader &shader) const
{
    shader.SetColor(color);

    UseMesh();

    glDrawElements(mesh.GetType(), mesh.GetElementsCnt(), GL_UNSIGNED_INT, nullptr);
}
