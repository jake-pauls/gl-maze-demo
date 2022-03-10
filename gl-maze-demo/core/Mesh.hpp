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
    struct MeshData {
        const float* vertices;
        int verticesSize;
        
        const float* normals;
        int normalsSize;
        
        const GLuint* indices;
        int numberOfIndices;
    };
    
    GLuint VAO;
    GLuint IBO;
    
    // Buffers
    GLuint vertexBuffer;
    GLuint normalBuffer;
    
    void SetupMesh(MeshData data);
};

#endif /* Mesh_hpp */
