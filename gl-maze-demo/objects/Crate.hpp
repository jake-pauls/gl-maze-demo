//
//  Crate.hpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#ifndef Crate_hpp
#define Crate_hpp

#include <chrono>

#include <OpenGLES/ES3/gl.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.hpp"
#include "Mesh.hpp"

class Crate
{
public:
    Crate();
    Crate(Mesh* mesh, glm::vec3 pos);
    void Update(glm::mat4 vpMatrix);
    void Draw(Shader* shaderProgram);
    
private:
    Mesh* _mesh;
    
    glm::mat4 _mvpMatrix, _modelMatrix;
    glm::mat3 _normalMatrix;
    
    glm::vec3 _pos;
    float _rot;
    
    std::chrono::time_point<std::chrono::steady_clock> _lastTime;
};

#endif /* Crate_hpp */
