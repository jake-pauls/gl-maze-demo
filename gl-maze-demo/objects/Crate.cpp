//
//  Crate.cpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#include <chrono>
#include <OpenGLES/ES3/gl.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "CrateData.h"

#include "Crate.hpp"
#include "Assert.hpp"

Crate::Crate() { }

Crate::Crate(glm::vec3 pos) : _pos(pos), _rot(0.0f)
{
    Mesh::SetupMesh(MeshData{
        CrateVertices,
        sizeof(CrateVertices),
        CrateNormals,
        sizeof(CrateNormals),
        CrateTextureCoords,
        sizeof(CrateTextureCoords),
        CrateIndices,
        sizeof(CrateIndices),
        NumberOfCrateIndices
    });
    
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
    
    _mvpMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0f));
    _modelMatrix = _mvpMatrix = glm::rotate(_mvpMatrix, _rot, glm::vec3(0.0, 1.0, 0.0));
    _normalMatrix = glm::inverseTranspose(glm::mat3(_mvpMatrix));
    
    _mvpMatrix = vpMatrix * _modelMatrix;
}

void Crate::Draw(Shader* shaderProgram)
{
    // Uniform Vectors
    glm::vec4 diffuseLightPosition(0.0f, 1.0f, 0.0f, 1.0f);
    glm::vec4 diffuseComponent(0.1f, 0.8f,0.1f, 1.0f);
    
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
    GL_CALL(glBindVertexArray(Mesh::VAO));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh::IBO));
    GL_CALL(glDrawElements(GL_TRIANGLES, NumberOfCrateIndices, GL_UNSIGNED_INT, 0));
    
    // Unbind vertex array
    GL_CALL(glBindVertexArray(0));
}
