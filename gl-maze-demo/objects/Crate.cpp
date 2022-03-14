//
//  Crate.cpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#include "Crate.hpp"
#include "Assert.hpp"

Crate::Crate() { }

Crate::Crate(Mesh* mesh, glm::vec3 pos)
    : _mesh(mesh), _pos(pos), _rot(0.0f)
{
    _lastTime = std::chrono::steady_clock::now();
}
    
void Crate::Update(glm::mat4 vpMatrix)
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - _lastTime).count();
    _lastTime = currentTime;
    
    _rot += 0.001f * elapsedTime;
    if (_rot >= 360.0f)
        _rot = 0.0f;
    
    _modelMatrix = glm::translate(glm::mat4(1.0f), _pos);
    _modelMatrix = glm::rotate(_modelMatrix, _rot, glm::vec3(0.0, 1.0, 0.0));
    _normalMatrix = glm::inverseTranspose(glm::mat3(_modelMatrix));
    
    _mvpMatrix = vpMatrix * _modelMatrix;
}

void Crate::Draw(Shader* shaderProgram)
{
    // Uniform Vectors
    glm::vec4 diffuseLightPosition(0.0f, 1.0f, 0.0f, 1.0f);
    glm::vec4 diffuseComponent(1.0f, 1.0f, 1.0f, 1.0f);
    
    // Set Uniforms
    shaderProgram->SetUniform1i("useTexture", 1);
    shaderProgram->SetUniform4fv("diffuseLightPosition", glm::value_ptr(diffuseLightPosition));
    shaderProgram->SetUniform4fv("diffuseComponent", glm::value_ptr(diffuseComponent));
    shaderProgram->SetUniformMatrix4fv("modelViewProjectionMatrix", glm::value_ptr(_mvpMatrix));
    shaderProgram->SetUniformMatrix4fv("modelViewMatrix", glm::value_ptr(_modelMatrix));
    shaderProgram->SetUniformMatrix3fv("normalMatrix", glm::value_ptr(_normalMatrix));
    
    // Use Program
    shaderProgram->Bind();
    
    // Bind vertex array and index buffer
    GL_CALL(glBindVertexArray(_mesh->VAO));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->IBO));
    GL_CALL(glDrawElements(GL_TRIANGLES, _mesh->NumberOfMeshIndices, GL_UNSIGNED_INT, 0));
    
    // Unbind vertex array
    GL_CALL(glBindVertexArray(0));
}
