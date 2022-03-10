//
//  Mesh.cpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#include "Mesh.hpp"
#include "Assert.hpp"

void Mesh::SetupMesh(MeshData data)
{
    LOG("Setting up mesh data...");
    
    GL_CALL(glGenVertexArrays(1, &VAO));
    GL_CALL(glBindVertexArray(VAO));
    
    // Position Data
    GL_CALL(glGenBuffers(1, &vertexBuffer));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, data.verticesSize, data.vertices, GL_STATIC_DRAW));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0));
    
    // Normals
    GL_CALL(glGenBuffers(1, &normalBuffer));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, normalBuffer));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, data.normalsSize, data.normals, GL_STATIC_DRAW));
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0));
    
    // Indices
    GL_CALL(glGenBuffers(1, &IBO));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * data.numberOfIndices, data.indices, GL_STATIC_DRAW));
    
    // Unbind vertex array
    GL_CALL(glBindVertexArray(0));
}
