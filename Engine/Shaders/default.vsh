#version 430

layout (location = 0) in vec4 vPosition;

out vec4 vertexColor;

vec4 vColor = vec4(1.0, 0.0, 0.0, 1.0);

void main() {
    gl_Position = vPosition;
    vertexColor = vColor;
}
