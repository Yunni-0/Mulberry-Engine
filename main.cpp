/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/* 
 * File:   main.cpp
 * Author: yelib
 *
 * Created on May 5, 2024, 8:04 PM
 */

#include <stdio.h>
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "include/Shader.h"
#include "include/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <algorithm>
#include <Camera.h>
#include <Light.h>
#include <Model.h>

float screenWidth = 800.0f;
float screenHeight = 800.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
Camera camera(0.0f, 0.0f, 1.0f);

namespace math {
    template <typename T>
    T clamp(T val, T lo, T hi) {
        if (val > hi) {
            return hi;
        }
        
        if (val < lo) {
            return lo;
        }
        
        return val;
    }
}

void resizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
}

void cursorCallback(GLFWwindow *window, double xPos, double yPos) {
    float deltaX = xPos - camera.lastCursorX;
    float deltaY = yPos - camera.lastCursorY;
    camera.lastCursorX = xPos;
    camera.lastCursorY = yPos;

    float sensitivity = 100.0f;
    deltaX *= sensitivity * deltaTime;
    deltaY *= sensitivity * deltaTime;

    camera.yaw += deltaX;
    camera.pitch -= deltaY;

    if (camera.pitch > 89.0f) {
        camera.pitch = 89.0f;
    }
    if (camera.pitch < -89.0f) {
        camera.pitch = -89.0f;
    }
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (glm::length(camera.cameraSpeed * camera.cameraFront) < camera.topSpeed) {
            camera.cameraSpeed += camera.acceleration * camera.cameraFront * deltaTime;
        }
    }
    
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (glm::length(camera.cameraSpeed * camera.cameraFront) < camera.topSpeed) {
            camera.cameraSpeed -= camera.acceleration * camera.cameraFront * deltaTime;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (glm::length(camera.cameraSpeed * camera.cameraRight) < camera.topSpeed) {
            camera.cameraSpeed -= camera.acceleration * camera.cameraRight * deltaTime;
        }
    }
    
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (glm::length(camera.cameraSpeed * camera.cameraRight) < camera.topSpeed) {
            camera.cameraSpeed += camera.acceleration * camera.cameraRight * deltaTime;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {
        if (glm::length(camera.cameraSpeed) > 0) {
            printf("speed: %f\n", glm::length(camera.cameraSpeed));
            camera.cameraSpeed -= camera.deceleration * glm::normalize(camera.cameraSpeed) * deltaTime;
        }
    }
}

/*
 * 
 */
int main(int argc, char** argv) {
    if (!glfwInit()) {
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow *window = glfwCreateWindow(800, 800, "Hey Window!", NULL, NULL);
    if (window == NULL) {
        puts("Couldn't create window!");
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cursorCallback);

    glfwGetCursorPos(window, &camera.lastCursorX, &camera.lastCursorY);
    
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        puts("Could not initialize GLAD!");
        glfwTerminate();
        return -1;
    }
    
    float verticies[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    Shader lighting("shader.vert", "lighting.frag");

    Model model("Mulberry.fbx");

    glViewport(0, 0, 800, 800);
    
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    
    glClearColor(0.5f, 0.0f, 0.7f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    
    glm::vec3 lightCubePos = glm::vec3(1.2f, 1.0f, 0.2f);
    //glm::vec3 lightCubePos = glm::vec3(-0.2f, -1.0f, -0.3f);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        camera.run(deltaTime);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
        
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), screenWidth/screenHeight, 0.1f, 100.0f);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        lighting.use();
        lighting.setVec4("light.pos", glm::vec4(lightCubePos, 1.0f));
        lighting.setVec3("cameraPos", camera.cameraPos);
        lighting.setMat4("view", view);
        lighting.setMat4("projection", projection);


        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

