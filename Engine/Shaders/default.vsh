#version 430

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;

out vec4 vertexColor;

void main() { 
    gl_Position = model * vPosition; 
    vertexColor = vColor;
}
