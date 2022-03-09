//
//  Mesh.hpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <OpenGLES/ES3/gl.h>

class Mesh
{
public:
    void SetupMesh(const float* positions, int positionsSize);
private:
    GLuint _VAO;
    GLuint _VBO;
};

#endif /* Mesh_hpp */
