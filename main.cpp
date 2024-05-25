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
    camera.pitch += deltaY;

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
            camera.cameraSpeed += camera.acceleration * camera.cameraFront;
        }
    }
    
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (glm::length(camera.cameraSpeed * camera.cameraFront) < camera.topSpeed) {
            camera.cameraSpeed -= camera.acceleration * camera.cameraFront;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (glm::length(camera.cameraSpeed * camera.cameraRight) < camera.topSpeed) {
            camera.cameraSpeed -= camera.acceleration * camera.cameraRight;
        }
    }
    
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (glm::length(camera.cameraSpeed * camera.cameraRight) < camera.topSpeed) {
            camera.cameraSpeed += camera.acceleration * camera.cameraRight;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {
        if (glm::length(camera.cameraSpeed) >= camera.deceleration) {
            printf("speed: %f\n", glm::length(camera.cameraSpeed));
            camera.cameraSpeed -= camera.acceleration * glm::normalize(camera.cameraSpeed);
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
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f
    };
    
    Shader lighting("shader.vert", "lighting.frag");
    Shader lightCube("shader.vert", "lightCube.frag");
    
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    
    glViewport(0, 0, 800, 800);
    
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    
    glClearColor(0.5f, 0.0f, 0.7f, 1.0f);
    
    glBindVertexArray(lightVAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    
    glEnable(GL_DEPTH_TEST);
    
    glm::vec3 lightCubePos = glm::vec3(1.2f, 1.0f, 0.2f);

    lighting.use();
    lighting.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    lighting.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

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
        
        glBindVertexArray(lightVAO);

        lighting.use();
        lighting.setMat4("view", view);
        lighting.setMat4("projection", projection);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        
        lighting.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightCube.use();
        lightCube.setMat4("view", view);
        lightCube.setMat4("projection", projection);

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightCubePos);
        model = glm::scale(model, glm::vec3(0.2f));

        lightCube.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

