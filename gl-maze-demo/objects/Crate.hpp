//
//  Crate.hpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#ifndef Crate_hpp
#define Crate_hpp

#include "glm/glm.hpp"

#include "Mesh.hpp"

class Crate : public Mesh
{
public:
    Crate(glm::vec3 pos);
private:
    glm::vec3 _pos;
    float _rot;
    
    void Update();
    void Draw();
};

#endif /* Crate_hpp */
