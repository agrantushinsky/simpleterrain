#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inTexCoord;

out vec3 TexCoord;

uniform mat4 matrix; 

void main()
{
    gl_Position = matrix * vec4(inPosition, 1.0);
    TexCoord = inTexCoord;
}

