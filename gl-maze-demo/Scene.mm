//
//  Scene.mm
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-08.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>

#import "Scene.h"
#import "TextureLoader.h"

#define GLM_FORCE_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <chrono>

#include "Assert.hpp"
#include "Shader.hpp"
#include "Crate.hpp"
#include "CrateData.h"

@interface Scene () {
    GLKView* viewport;
    std::chrono::time_point<std::chrono::steady_clock> lastTime;

    // OpenGL IDs
    GLuint crateTexture;

    // global lighting parameters
    glm::vec4 specularLightPosition;
    glm::vec4 specularComponent;
    GLfloat shininess;
    glm::vec4 ambientComponent;

    Shader* _shaderProgram;
    Crate* _crate;
    
    glm::mat4 _projectionMatrix;
    glm::mat4 _viewMatrix;
}

@end

@implementation Scene

@synthesize useFog;

/// Called when 'Scene' is deallocated
- (void)dealloc
{ }

/// Sets up GL context, shaders, and uniforms
- (void)setup:(GLKView *)view
{
    view.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    ASSERT(view.context);
    
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    viewport = view;
    [EAGLContext setCurrentContext:view.context];
    
    ASSERT([self setupShaders]);

    // Texture and Fog Uniforms
    useFog = 0;
    crateTexture = [TextureLoader loadTextureFile:@"crate.jpg"];
    GL_CALL(glActiveTexture(GL_TEXTURE0));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, crateTexture));

    // Lighting Values
    shininess = 1000.0f;
    specularComponent = glm::vec4(0.8f, 0.1f, 0.1f, 1.0f);
    specularLightPosition = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    ambientComponent = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

    GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
    GL_CALL(glEnable(GL_DEPTH_TEST));
    GL_CALL(glEnable(GL_CULL_FACE));
    
    lastTime = std::chrono::steady_clock::now();
}

/// Loads Meshes for models in the scene
- (void)loadModels
{
    _crate = new Crate(glm::vec3(0.0f, 0.0f, 0.0f));
}

/// Update is called once per frame
- (void)update
{
    // perspective projection matrix
    float aspectRatio = (float) viewport.drawableWidth / (float) viewport.drawableHeight;
    _projectionMatrix = glm::perspective(glm::radians(60.0f), aspectRatio, 1.0f, 20.0f);
    
    _viewMatrix = glm::lookAt(
        glm::vec3(0, 2, 3),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );
    
    // Update Crate Object
    _crate->Update(_projectionMatrix * _viewMatrix);
}

/// Draw is called once per frame
- (void)draw;
{
    // pass on global lighting, fog and texture values
    _shaderProgram->SetUniform4fv("specularLightPosition", glm::value_ptr(specularLightPosition));
    _shaderProgram->SetUniform1f("shininess", shininess);
    _shaderProgram->SetUniform4fv("specularComponent", glm::value_ptr(specularComponent));
    _shaderProgram->SetUniform4fv("ambientComponent", glm::value_ptr(ambientComponent));
    _shaderProgram->SetUniform1i("useFog", useFog);

    GL_CALL(glViewport(0, 0, (int)viewport.drawableWidth, (int)viewport.drawableHeight));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    
    // Draw Crate Object
    _crate->Draw(_shaderProgram);
}

/// Sets up the basic program object for the scene
- (bool)setupShaders
{
    _shaderProgram = new Shader([self retrieveFilePathByName:"Shader.vsh"], [self retrieveFilePathByName:"Shader.fsh"]);

    return true;
}

/// Retrieves resources within the Xcode project
- (const char*)retrieveFilePathByName:(const char*)fileName
{
    return [[[NSBundle mainBundle] pathForResource:[[NSString stringWithUTF8String:fileName] stringByDeletingPathExtension] ofType:[[NSString stringWithUTF8String:fileName] pathExtension]] cStringUsingEncoding:1];
}

@end
