//
//  PlaneData.h
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-11.
//

#ifndef PlaneData_h
#define PlaneData_h

#include <OpenGLES/ES3/gl.h>

const int NumberOfPlaneVertices = 4;
const int NumberOfPlaneIndices = 6;

const float PlaneVertices[12] =
{
    -0.5f, -0.5f, 0.0f,
    0.5f, 0.5f,  0.0f,
    -0.5f, 0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
};

const float PlaneNormals[12] =
{
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
};

const float PlaneTextureCoords[8] =
{
    0.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
};

GLuint PlaneIndices[6] =
{
    0, 1, 2,
    0, 3, 1,
};

#endif /* PlaneData_h */
