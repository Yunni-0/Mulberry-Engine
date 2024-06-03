#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

struct Light {
    glm::vec3 pos;

    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;
};
#endif