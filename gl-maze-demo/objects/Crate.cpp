//
//  Crate.cpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#include <OpenGLES/ES3/gl.h>
#include <glm/gtx/transform.hpp>

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
        CrateIndices,
        NumberOfCrateIndices
    });
}
    
void Crate::Update()
{
    _rot += 0.01;
}

void Crate::Draw(Shader* shaderProgram, glm::mat4 vpMatrix)
{
    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, _pos);
    modelMatrix = glm::rotate(modelMatrix, _rot, glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::mat4 mvpMatrix = modelMatrix * vpMatrix;
    
    // Set Uniforms
    shaderProgram->SetUniform4f("_color", 0.0f, 1.0f, 0.0f, 1.0f);
    shaderProgram->SetUniformMatrix4fv("_mvpMatrix", &mvpMatrix[0][0]);
    
    // Use Program
    shaderProgram->Bind();
    
    // Bind vertex array and index buffer
    GL_CALL(glBindVertexArray(Mesh::VAO));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh::IBO));
    GL_CALL(glDrawElements(GL_TRIANGLES, static_cast<GLuint>(NumberOfCrateIndices), GL_UNSIGNED_INT, 0));
    
    GL_CALL(glBindVertexArray(0));
}
