#shader vertex
#version 300 es

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoordIn;

out vec3 normalPos;
out vec4 fragPos;
out vec2 texCoords;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

void main()
{
    // Calculate normal vector in eye coordinates
    normalPos = (normalMatrix * normal);
    
    // Calculate vertex position in view coordinates
    fragPos = modelViewMatrix * position;
    
    // Pass through texture coordinate
    texCoords = texCoordIn;

    // Set gl_Position with transformed vertex position
    gl_Position = modelViewProjectionMatrix * position;
}
