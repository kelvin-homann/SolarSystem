#ifndef _LIGHT_H
#define _LIGHT_H

#include <glm/glm.hpp>

class Light {
    public:
        Light();
        glm::vec3 position = glm::vec3(0.f);
        glm::vec3 color = glm::vec3(0.f);
        glm::vec3 diffuse = glm::vec3(0.f);
        glm::vec3 specular = glm::vec3(0.f);
};

#endif
