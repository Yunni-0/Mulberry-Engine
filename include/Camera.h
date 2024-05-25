#ifndef CAMERA
#define CAMERA

#include <glm/glm.hpp>

class Camera {
public:
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraRight = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraFront);
    glm::vec3 cameraUp = glm::cross(cameraFront, cameraRight);
    glm::vec3 cameraSpeed = glm::vec3(0.0f);
    float topSpeed = 20.0f;
    float acceleration = 5.0f;
    float deceleration = 2.5f;
    double lastCursorX = 400.0;
    double lastCursorY = 400.0;
    float pitch = 0.0f;
    float yaw = -90.0f;

    Camera(float x, float y, float z) {
        cameraPos = glm::vec3(x, y, z);
    }

    void run(float deltaTime) {
        cameraPos += cameraSpeed * deltaTime;

        cameraFront = glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
        cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraFront));
    }
};

#endif