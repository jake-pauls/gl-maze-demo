//
//  Wall.hpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-11.
//

#ifndef Wall_hpp
#define Wall_hpp

#include <chrono>

#include <OpenGLES/ES3/gl.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.hpp"
#include "Mesh.hpp"

class Wall : public Mesh
{
public:
    Wall();
    Wall(glm::vec3 pos);
    Wall(glm::vec3 pos, glm::vec3 rot, float rotAngle);
    void Draw(Shader* shaderProgram, glm::mat4 vpMatrix);
    
private:
    glm::mat4 _mvpMatrix, _modelMatrix;
    glm::mat3 _normalMatrix;
    
    glm::vec3 _pos, _rot;
    float _rotAngle;
};


#endif /* Wall_hpp */
