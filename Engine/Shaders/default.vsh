#version 430

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor;

void main() { 
    gl_Position = projection * view * model * vPosition; 
    vertexColor = vColor;
}
