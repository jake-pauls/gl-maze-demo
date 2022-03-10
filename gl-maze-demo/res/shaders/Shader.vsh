#shader vertex
#version 300 es

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec4 v_color;

uniform mat4 _mvpMatrix;
uniform vec4 _color;

void main()
{
    // Simple passthrough shader
    v_color = _color;

    gl_Position = _mvpMatrix * position;
}
