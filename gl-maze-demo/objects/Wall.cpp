//
//  Wall.cpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-11.
//

#include <OpenGLES/ES3/gl.h>

#include "Wall.hpp"
#include "Assert.hpp"

Wall::Wall() { }

/// Creates a wall with a given position, no rotation by default
/// Front of wall faces positive z-direction
Wall::Wall(Mesh* mesh, glm::vec3 pos)
    : _mesh(mesh), _pos(pos), _rotAngle(0.0f)
{ }


/// Creates a wall with both a given position and rotation about an axis
Wall::Wall(Mesh* mesh, glm::vec3 pos, float rotAngle)
    : _mesh(mesh), _pos(pos), _rotAngle(rotAngle)
{ }

void Wall::Draw(Shader *shaderProgram, glm::mat4 vpMatrix)
{
    // Calculate initial mvp matrix
    _modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    _modelMatrix = glm::translate(_modelMatrix, _pos);
    _modelMatrix = glm::rotate(_modelMatrix, _rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    
    _normalMatrix = glm::inverseTranspose(glm::mat3(_modelMatrix));
    
    _mvpMatrix = vpMatrix * _modelMatrix;
    
    // Uniform Vectors
    glm::vec4 diffuseLightPosition(0.5f, 0.5f, 0.5f, 1.0f);
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
    
    // Render the same plane on the opposite side
    // Allows easy re-texturing of either side of the plane
    // TODO: Rudimentary and/or bugged flipping logic (works for 0 deg, 90 deg)
    
    glm::vec3 flipVector(1.0f, 1.0f, 1.0f);
    
    if (_rotAngle == 0.0f) {
        flipVector.x *= -1.0f;
    } else if (_rotAngle > 0.0f && _rotAngle <= 90.0f) {
        flipVector.z *= -1.0f;
    } else if (_rotAngle > 90.0f && _rotAngle <= 180.0f) {
        flipVector.x *= -1.0f;
    } else if (_rotAngle > 180.0f && _rotAngle <= 270.0f) {
        flipVector.z *= -1.0f;
    } else if (_rotAngle > 270.0f && _rotAngle <= 360.0f) {
        flipVector.x *= 1.0f;
    }
    
    _modelMatrix = glm::scale(glm::mat4(1.0f), flipVector);
    _modelMatrix = glm::translate(_modelMatrix, _pos);
    _modelMatrix = glm::rotate(_modelMatrix, _rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    
    _normalMatrix = glm::inverseTranspose(glm::mat3(_modelMatrix));
    
    _mvpMatrix = vpMatrix * _modelMatrix;
    
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
