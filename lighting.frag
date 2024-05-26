#version 330 core
out vec4 fragmentColor;
in vec3 normal;
in vec3 pos;
in vec2 texCoord;

struct Mat {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 pos;

    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};

uniform Mat material;
uniform Light light;
uniform vec3 cameraPos;

void main () {
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.pos - pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = (diff * vec3(texture(material.diffuse, texCoord))) * light.diffuse;

    vec3 viewDir = normalize(cameraPos - pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularLight = (vec3(texture(material.specular, texCoord)) * spec) * light.specular;

    vec3 ambientLight = light.ambient * vec3(texture(material.diffuse, texCoord));

    vec3 result = ambientLight + diffuseLight + specularLight;
    fragmentColor = vec4(result, 1.0);
}