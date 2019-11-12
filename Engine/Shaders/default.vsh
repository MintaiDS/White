#version 430

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

layout(location = 2) uniform mat4 model;

out vec4 vertexColor;

void main() {
    gl_Position = model * vPosition;
    vertexColor = vColor;
}
