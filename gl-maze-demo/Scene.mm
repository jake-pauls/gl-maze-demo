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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Maze.h"
#include "CrateData.h"
#include "PlaneData.h"

#include "Assert.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Crate.hpp"
#include "Floor.hpp"
#include "Wall.hpp"

@interface Scene () {
    GLKView* _viewport;

    // Textures
    GLuint _crateTexture;
    GLuint _grassTexture;

    Shader* _shaderProgram;
    
    // Global Lighting Parameters
    glm::vec4 _specularLightPosition;
    glm::vec4 _specularComponent;
    GLfloat _shininess;
    glm::vec4 _ambientComponent;
    
    // Meshes
    Mesh* _cubeMesh;
    Mesh* _planeMesh;
    
    // Objects
    Crate* _crate;
    Floor* _floor;
    Wall* _wall;
    
    // Wall Generation
    std::vector<Wall*> _wallList;
    
    // MVP Matrices
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
    _shininess = 1000.0f;
    _specularComponent = glm::vec4(0.8f, 0.1f, 0.1f, 1.0f);
    _specularLightPosition = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    _ambientComponent = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    
    // Meshes
    _cubeMesh = new Mesh();
    _planeMesh = new Mesh();
    
    GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
    GL_CALL(glEnable(GL_DEPTH_TEST));
    GL_CALL(glEnable(GL_CULL_FACE));
}

/// Loads Meshes for models in the scene
/// 'Mesh' contains the relevant vertex data for rendering
- (void)loadModels
{
    ASSERT([self loadMeshes]);
    
    // Create objects
    _crate = new Crate(_cubeMesh, glm::vec3(1.0f, 0.0f, 0.0f));
    _wall = new Wall(_planeMesh, glm::vec3(0.0f, 0.0f, 0.0f), 180.0f);
    _floor = new Floor(_cubeMesh, glm::vec3(0.0f, -1.0f, 0.0f));
    
    // Create new maze
    // 0 - False, 1 - True
    _maze = new Maze();
    _maze->Create();
    
    // Maze Debug
    int i, j;
    int numRows = 4, numCols = 4;
   
    for (i=numRows-1; i>=0; i--) {
        for (j=numCols-1; j>=0; j--) {    // top
            printf(" %c ", _maze->GetCell(i, j).southWallPresent ? '-' : ' ');
        }
        printf("\n");
        for (j=numCols-1; j>=0; j--) {    // left/right
            printf("%c", _maze->GetCell(i, j).eastWallPresent ? '|' : ' ');
            printf("%c", ((i+j) < 1) ? '*' : ' ');
            printf("%c", _maze->GetCell(i, j).westWallPresent ? '|' : ' ');
        }
        printf("\n");
        for (j=numCols-1; j>=0; j--) {    // bottom
            printf(" %c ", _maze->GetCell(i, j).northWallPresent ? '-' : ' ');
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
    float aspectRatio = (float) _viewport.drawableWidth / (float) _viewport.drawableHeight;
    _projectionMatrix = glm::perspective(glm::radians(60.0f), aspectRatio, 1.0f, 20.0f);
    
    _viewMatrix = glm::lookAt(
        glm::vec3(0, 0.5, -4),     // Camera is Positioned Here
        glm::vec3(0, 0.5, 0),     // Camera Looks at this Point
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
    _shaderProgram->SetUniform4fv("specularLightPosition", glm::value_ptr(_specularLightPosition));
    _shaderProgram->SetUniform1f("shininess", _shininess);
    _shaderProgram->SetUniform4fv("specularComponent", glm::value_ptr(_specularComponent));
    _shaderProgram->SetUniform4fv("ambientComponent", glm::value_ptr(_ambientComponent));
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

/// Sets up the meshes with relevant render data
- (bool)loadMeshes
{
    // Generate cube mesh using CrateData
    _cubeMesh->SetupMesh(Mesh::MeshData{
        CrateVertices,
        sizeof(CrateVertices),
        CrateNormals,
        sizeof(CrateNormals),
        CrateTextureCoords,
        sizeof(CrateTextureCoords),
        CrateIndices,
        sizeof(CrateIndices),
        NumberOfCrateIndices
    });
    
    // Generate plane mesh using PlaneData
    _planeMesh->SetupMesh(Mesh::MeshData{
        PlaneVertices,
        sizeof(PlaneVertices),
        PlaneNormals,
        sizeof(PlaneNormals),
        PlaneTextureCoords,
        sizeof(PlaneTextureCoords),
        PlaneIndices,
        sizeof(PlaneIndices),
        NumberOfPlaneIndices
    });
    
    return true;
}

/// Retrieves resources within the Xcode project
- (const char*)retrieveFilePathByName:(const char*)fileName
{
    return [[[NSBundle mainBundle] pathForResource:[[NSString stringWithUTF8String:fileName] stringByDeletingPathExtension] ofType:[[NSString stringWithUTF8String:fileName] pathExtension]] cStringUsingEncoding:1];
}

@end
