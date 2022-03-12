//
//  Floor.hpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-11.
//

#ifndef Floor_hpp
#define Floor_hpp

#include <OpenGLES/ES3/gl.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.hpp"
#include "Mesh.hpp"

class Floor
{
public:
    Floor();
    Floor(Mesh* mesh, glm::vec3 pos);
    void Draw(Shader* shaderProgram, glm::mat4 vpMatrix);
    
private:
    Mesh* _mesh;
    
    glm::mat4 _mvpMatrix, _modelMatrix;
    glm::mat3 _normalMatrix;
    
    glm::vec3 _pos;
};

#endif /* Floor_hpp */
