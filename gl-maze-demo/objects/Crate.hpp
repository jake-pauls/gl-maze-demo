//
//  Crate.hpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#ifndef Crate_hpp
#define Crate_hpp

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Mesh.hpp"

class Crate : public Mesh
{
public:
    Crate();
    Crate(glm::vec3 pos);
    void Update();
    void Draw(Shader* shaderProgram, glm::mat4 vpMatrix);
    
private:
    glm::vec3 _pos;
    float _rot;
};

#endif /* Crate_hpp */
