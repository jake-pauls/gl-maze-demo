//
//  Scene.mm
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#import "Scene.h"

#include "Shader.hpp"
#include "Crate.hpp"
#include "Assert.hpp"

@interface Scene()
{
    GLKView* viewport;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    
    // Uniforms for program
    enum
    {
        UNIFORM_MVP_MATRIX,
        NUM_UNIFORMS
    };
    
    GLint uniforms[NUM_UNIFORMS];
    GLuint program;
}

@end

@implementation Scene

/// Constructs the core scene
- (id)init
{
    LOG("Constructed the main scene");
    
    Crate(glm::vec3(0.0f, 0.0f, 0.0f));
    
    return self;
}

- (void)load
{
    
}

- (void)update
{
    float aspectRatio = (float) viewport.drawableWidth / (float) viewport.drawableHeight;
    
    projectionMatrix = glm::perspective(glm::radians(60.0f), aspectRatio, 1.0f, 20.0f);
    
    // Look at will 'move' the camera, these coordinates could be replaced by movement
    viewMatrix = glm::lookAt(
        glm::vec3(2, 2, 2),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );
}

- (void)draw
{
    // Clears the viewport after each draw call
    GL_CALL(glViewport(0, 0, (int) viewport.drawableWidth, (int) viewport.drawableHeight));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    
}

/// Sets up the GL view
- (void)setup:(GLKView*)view
{
    // Setup the view context
    view.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    ASSERT(view.context);
    
    [EAGLContext setCurrentContext:view.context];
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    viewport = view;
    
    // Setup shaders
    ASSERT([self setupShaders]);
    
    GL_CALL(glClearColor(0, 0, 0, 0));
    GL_CALL(glEnable(GL_DEPTH_TEST));
    GL_CALL(glEnable(GL_CULL_FACE));
    GL_CALL(glEnable(GL_BLEND));
}

/// Sets up the basic program object for the scene
- (bool)setupShaders
{
    Shader::ProgramSource shaderSource;
    shaderSource.vertexSource = Shader().parseShader([self retrieveFilePathByName:"Shader.vsh"]).vertexSource;
    shaderSource.fragmentSource = Shader().parseShader([self retrieveFilePathByName:"Shader.fsh"]).fragmentSource;
    
    GLuint programObject = Shader().createShader(shaderSource.vertexSource, shaderSource.fragmentSource);
    GL_CALL(glUseProgram(programObject));
    ASSERT(programObject != 0)
    
    // Retrieve uniforms
    uniforms[UNIFORM_MVP_MATRIX] = glGetUniformLocation(programObject, "_mvpMatrix");
    
    // Set program object
    program = programObject;
    
    return true;
}

- (const char*)retrieveFilePathByName:(const char*)fileName
{
    return [[[NSBundle mainBundle] pathForResource:[[NSString stringWithUTF8String:fileName] stringByDeletingPathExtension] ofType:[[NSString stringWithUTF8String:fileName] pathExtension]] cStringUsingEncoding:1];
}

@end
