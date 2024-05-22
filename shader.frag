#version 330 core
out vec4 fragmentColor;
in vec2 texCoord;
in vec3 pos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixVal;

void main () {
    fragmentColor = vec4(pos, 1.0);
}