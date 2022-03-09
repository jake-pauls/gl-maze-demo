//
//  Mesh.cpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#include "Mesh.hpp"
#include "Assert.hpp"

void Mesh::SetupMesh(const float* positions, int positionsSize)
{
    LOG("Setting up mesh...");
    
    GL_CALL(glGenVertexArrays(1, &_VAO));
    GL_CALL(glBindVertexArray(_VAO));
    
    GL_CALL(glGenBuffers(1, &_VBO));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _VBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, positionsSize, positions, GL_STATIC_DRAW));
    
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL));
    GL_CALL(glEnableVertexAttribArray(0));
    
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL));
    GL_CALL(glEnableVertexAttribArray(1));
}
