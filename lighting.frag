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
    vec4 pos;

    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};

uniform Mat material;
uniform Light light;
uniform vec3 cameraPos;

void main () {
    vec3 norm = normalize(normal);
    vec3 lightDir;
    float attenuation;

    if (light.pos.w == 0.0) {
        attenuation = 1.0;
        lightDir = normalize(vec3(-light.pos));
    }
    
    else if (light.pos.w == 1.0) {
        float distance = length(vec3(light.pos) - pos);
        attenuation = 1.0/(1.0+0.09*distance*0.032*(distance*distance));

        lightDir = normalize(vec3(light.pos) - pos);
    }

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = (diff * vec3(texture(material.diffuse, texCoord))) * light.diffuse;

    vec3 viewDir = normalize(cameraPos - pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularLight = (vec3(texture(material.specular, texCoord)) * spec) * light.specular;

    vec3 ambientLight = light.ambient * vec3(texture(material.diffuse, texCoord));

    vec3 result = (ambientLight + diffuseLight + specularLight) * attenuation;
    fragmentColor = vec4(result, 1.0);
}