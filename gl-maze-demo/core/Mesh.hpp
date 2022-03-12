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
        float verticesSize;
        
        const float* normals;
        float normalsSize;
        
        const float* textures;
        float texturesSize;
        
        const GLuint* indices;
        float indicesSize;
        int numberOfIndices;
    };
    
    GLuint VAO;
    GLuint VBO, NBO, TBO, IBO;     // Buffer Objects (vertex, normal, texture, index)
    
    int NumberOfMeshIndices;
    
    void SetupMesh(MeshData data);
};

#endif /* Mesh_hpp */
