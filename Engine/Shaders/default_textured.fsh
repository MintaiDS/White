#version 430 

in vec4 vertexColor;
in vec2 texCoord;

out vec4 fragmentColor;

uniform sampler2D modelTexture;

void main() {
    fragmentColor = texture(modelTexture, texCoord);
}
