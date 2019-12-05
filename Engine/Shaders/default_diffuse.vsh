#version 430

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec4 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor;
out vec4 vertexNormal;
out vec4 fragmentPos;

void main() { 
    gl_Position = projection * view * model * vPosition; 
    fragPos = mode * vPosition;
    vertexNormal = vNormal;
}
