#version 330 core
out vec4 fragmentColor;
in vec3 normal;
in vec3 pos;
in vec2 texCoord;

uniform vec3 lightColor;

void main() {
    fragmentColor = vec4(lightColor, 1.0);
}