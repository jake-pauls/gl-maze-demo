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
    
    // Set local indices count
    NumberOfMeshIndices = data.numberOfIndices;
    
    GL_CALL(glGenVertexArrays(1, &VAO));
    GL_CALL(glBindVertexArray(VAO));
    
    // Position Data
    GL_CALL(glGenBuffers(1, &VBO));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, data.verticesSize, data.vertices, GL_STATIC_DRAW));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0));
    
    // Normals
    GL_CALL(glGenBuffers(1, &NBO));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, NBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, data.normalsSize, data.normals, GL_STATIC_DRAW));
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0));
    
    // Textures
    GL_CALL(glGenBuffers(1, &TBO));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, TBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, data.texturesSize, data.textures, GL_STATIC_DRAW));
    GL_CALL(glEnableVertexAttribArray(2));
    GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *) 0));
    
    // Indices
    GL_CALL(glGenBuffers(1, &IBO));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indicesSize, data.indices, GL_STATIC_DRAW));
    
    // Unbind vertex array
    GL_CALL(glBindVertexArray(0));
}
