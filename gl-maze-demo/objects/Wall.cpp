//
//  Wall.cpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-11.
//

#include <OpenGLES/ES3/gl.h>

#include "PlaneData.h"

#include "Wall.hpp"
#include "Assert.hpp"

Wall::Wall() { }

/// Creates a wall with a given position, no rotation by default
/// Front of wall faces positive z-direction
Wall::Wall(glm::vec3 pos)
    : _pos(pos), _rot(glm::vec3(1.0f, 1.0f, 1.0f)), _rotAngle(0.0f)
{
    Mesh::SetupMesh(MeshData{
        PlaneVertices,
        sizeof(PlaneVertices),
        PlaneNormals,
        sizeof(PlaneNormals),
        PlaneTextureCoords,
        sizeof(PlaneTextureCoords),
        PlaneIndices,
        sizeof(PlaneIndices),
        NumberOfPlaneIndices
    });
}


/// Creates a wall with both a given position and rotation about an axis
Wall::Wall(glm::vec3 pos, glm::vec3 rot, float rotAngle)
    : _pos(pos), _rot(rot), _rotAngle(rotAngle)
{
    Mesh::SetupMesh(MeshData{
        PlaneVertices,
        sizeof(PlaneVertices),
        PlaneNormals,
        sizeof(PlaneNormals),
        PlaneTextureCoords,
        sizeof(PlaneTextureCoords),
        PlaneIndices,
        sizeof(PlaneIndices),
        NumberOfPlaneIndices
    });
}

void Wall::Draw(Shader *shaderProgram, glm::mat4 vpMatrix)
{
    // Calculate initial mvp matrix
    _modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    _modelMatrix = glm::translate(_modelMatrix, _pos);
    _modelMatrix = glm::rotate(_modelMatrix, _rotAngle, _rot);
    
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
    GL_CALL(glDrawElements(GL_TRIANGLES, NumberOfPlaneIndices, GL_UNSIGNED_INT, 0));
    
    // Unbind vertex array
    GL_CALL(glBindVertexArray(0));
}
