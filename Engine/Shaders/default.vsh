#version 430

layout (location = 0) in vec4 vPosition;

out vec4 vertexColor;

vec4 vColor = vec4(1.0, 0.0, 0.0, 1.0);

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition;
    vertexColor = vColor;
}
