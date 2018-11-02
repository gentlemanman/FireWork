#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out float curAlpha;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 modelMat[100];
uniform float alpha[100];

void main()
{
    TexCoords = aTexCoords;
    mat4 model = modelMat[gl_InstanceID];
    curAlpha = alpha[gl_InstanceID];
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //gl_Position = projection * view * model * vec4(aPos, 1.0);
}