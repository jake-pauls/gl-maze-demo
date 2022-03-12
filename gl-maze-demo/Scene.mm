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

#include <iostream>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Maze.h"

#include "Assert.hpp"
#include "Shader.hpp"
#include "Crate.hpp"
#include "Floor.hpp"
#include "Wall.hpp"

@interface Scene () {
    GLKView* _viewport;
    std::chrono::time_point<std::chrono::steady_clock> _lastTime;

    // Textures
    GLuint _crateTexture;
    GLuint _grassTexture;

    // Global Lighting Parameters
    glm::vec4 specularLightPosition;
    glm::vec4 specularComponent;
    GLfloat shininess;
    glm::vec4 ambientComponent;

    Shader* _shaderProgram;
    Crate* _crate;
    Floor* _floor;
    Wall* _wall;
    
    // Wall Generation
    std::vector<Wall*> _wallList;
    
    glm::mat4 _projectionMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _viewProjectionMatrix;
    
    Maze* _maze;
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
    _viewport = view;
    [EAGLContext setCurrentContext:view.context];
    
    ASSERT([self setupShaders]);

    // Fog Uniform
    useFog = 0;
    
    // Load in textures
    _crateTexture = [TextureLoader loadTextureFile:@"crate.jpg"];
    _grassTexture = [TextureLoader loadTextureFile:@"grass.jpg"];

    // Lighting Values
    shininess = 1000.0f;
    specularComponent = glm::vec4(0.8f, 0.1f, 0.1f, 1.0f);
    specularLightPosition = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    ambientComponent = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

    GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
    GL_CALL(glEnable(GL_DEPTH_TEST));
    GL_CALL(glEnable(GL_CULL_FACE));
    
    _lastTime = std::chrono::steady_clock::now();
}

/// Loads Meshes for models in the scene
- (void)loadModels
{
    _crate = new Crate(glm::vec3(-1.0f, 1.0f, 0.0f));
    _wall = new Wall(glm::vec3(0.0f, 1.0f, 1.0f));
    _floor = new Floor(glm::vec3(0.0f, 0.0f, 0.0f));
    
    // Create new maze
    // 0 - False, 1 - True
    _maze = new Maze();
    _maze->Create();
    
    // Maze Debug
    int i, j;
    int numRows = 4, numCols = 4;
    
    for (i = numRows - 1; i >= 0; i--) {
        for (j = numCols - 1; j >= 0; j--) {
            printf("%c", _maze->GetCell(i, j).southWallPresent ? '-' : ' ');
        }
        
        printf("\n");
        
        for (j = numCols - 1; j >= 0; j--) {
            printf("%c", _maze->GetCell(i, j).eastWallPresent ? '|' : ' ');
            printf("%c", ((i + j) < 1) ? '*' : ' ');
            printf("%c", _maze->GetCell(i, j).westWallPresent ? '|' : ' ');
        }
        
        printf("\n");
        
        for (j = numCols - 1; j >= 0; j--) {
            printf("%c", _maze->GetCell(i, j).northWallPresent ? '-' : ' ');
        }
        
        printf("\n");
    }
    
    for (int i = 0; i < _maze->rows; i++)
    {
        for (int j = 0; j < _maze->cols; j++)
        {
            // MazeCell cell = _maze->GetCell(i, j);
            
            // float wallOffset = 0.45;
            // _cellPosition = glm::vec3(i, 1.0f, j);
            
            // if (cell.northWallPresent) {
            //    _wallPosition = _cellPosition;
            //    _wallPosition.z += wallOffset;
            //    _wallList.push_back(new Wall(_wallPosition));
            // }
        }
    }
}

/// Update is called once per frame
- (void)update
{
    // perspective projection matrix
    float aspectRatio = (float) _viewport.drawableWidth / (float) _viewport.drawableHeight;
    _projectionMatrix = glm::perspective(glm::radians(60.0f), aspectRatio, 1.0f, 20.0f);
    
    _viewMatrix = glm::lookAt(
        glm::vec3(0, 2, 8),    // Camera is Positioned Here
        glm::vec3(0, 2, 0),     // Camera Looks at this Point
        glm::vec3(0, 1, 0)
    );
    
    _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
    
    // Update Crate Object
    _crate->Update(_viewProjectionMatrix);
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

    GL_CALL(glViewport(0, 0, (int)_viewport.drawableWidth, (int)_viewport.drawableHeight));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    
    // Draw Crate
    GL_CALL(glBindTexture(GL_TEXTURE_2D, _crateTexture));
    _crate->Draw(_shaderProgram);
    
    // Draw test Wall
    _wall->Draw(_shaderProgram, _viewProjectionMatrix);
    
    // Draw walls
    // for (int i = 0; i < _wallList.size(); i++)
    //    _wallList[i]->Draw(_shaderProgram, _viewProjectionMatrix);
    
    // Draw Floor
    GL_CALL(glBindTexture(GL_TEXTURE_2D, _grassTexture));
    _floor->Draw(_shaderProgram, _viewProjectionMatrix);
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
