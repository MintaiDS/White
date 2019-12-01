#version 430 

in vec4 vertexColor;
in vec2 texCoord;

out vec4 fragmentColor;

uniform sampler2D modelTexture;

void main() {
    //fragmentColor = vec4(1.0, 1.0, 0.0, 1.0);
    fragmentColor = texture(modelTexture, texCoord);
}
