#version 330 core
out vec4 fragmentColor;
in vec3 normal;
in vec3 pos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main () {
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(cameraPos - pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specularLight = specularStrength * spec * lightColor;

    float ambientStrength = 0.1f;
    vec3 ambientLight = lightColor * ambientStrength;

    vec3 result = objectColor * (ambientLight + diffuseLight + specularLight);
    fragmentColor = vec4(result, 1.0);
}