//
//  Crate.cpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#include "glm/gtx/transform.hpp"

#include "CrateData.h"

#include "Crate.hpp"
#include "Assert.hpp"

Crate::Crate(glm::vec3 pos) : _pos(pos), _rot(0.0f)
{
    Mesh::SetupMesh(CratePositions, sizeof(CratePositions));
}
    
void Crate::Update()
{
    _rot += 0.01;
}

void Crate::Draw()
{
    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, _pos);
    modelMatrix = glm::rotate(modelMatrix, _rot, glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Set Uniforms
    
    // Use Program
    
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, CrateVertices));
}
