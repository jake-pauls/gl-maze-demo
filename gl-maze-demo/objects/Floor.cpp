//
//  Floor.cpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-11.
//

#include <OpenGLES/ES3/gl.h>

#include "CrateData.h"

#include "Floor.hpp"
#include "Assert.hpp"

Floor::Floor() { }

Floor::Floor(glm::vec3 pos) : _pos(pos)
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
}

void Floor::Draw(Shader *shaderProgram, glm::mat4 vpMatrix)
{
    // Calculate initial mvp matrix
    _modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(12.0f, 1.0f, 12.0f));
    _modelMatrix = glm::translate(_modelMatrix, _pos);
    _modelMatrix = glm::rotate(_modelMatrix, 0.0f, glm::vec3(0.0, 1.0, 0.0));
    
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
    GL_CALL(glBindVertexArray(Mesh::VAO));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh::IBO));
    GL_CALL(glDrawElements(GL_TRIANGLES, NumberOfCrateIndices, GL_UNSIGNED_INT, 0));
    
    // Unbind vertex array
    GL_CALL(glBindVertexArray(0));
}
