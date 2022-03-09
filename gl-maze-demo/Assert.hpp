//
//  Assert.hpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#ifndef Assert_hpp
#define Assert_hpp

#include <iostream>
#include <stdio.h>
#include <OpenGLES/ES3/gl.h>

#define LOG(x) \
    std::cout << "[gl-maze-demo::" << strrchr("/" __FILE__, '/') + 1 << "(" << __LINE__ << ")]" << x << std::endl;

#define ASSERT(x) if (!(x)) {}

#define GL_CALL(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearErrors();
bool GLLogCall(const char* function, const char* file, int lineNumber);

#endif /* Assert_hpp */
