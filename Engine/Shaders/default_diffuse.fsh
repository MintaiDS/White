#version 430 

in vec4 vertexColor;
in vec4 vertexNormal;
in vec4 fragmentPos;

out vec4 fragmentColor;

uniform vec4 lighPosition;
uniform vec4 lightColor;

void main() {
    vec4 norm = normalize(vertexNormal);
    vec4 lightDirection = normalize(lighPosition - fragmentPos);
    float diff = max(dot(norm, lighDirection), 0.0);
    vec4 diffuseLight = diff * lightColor;
    vec4 resultColor = (ambient + diffuse) * vertexColor;
    fragmentColor = resultColor;
}
