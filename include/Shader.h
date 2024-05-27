/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   Shader.h
 * Author: yelib
 *
 * Created on May 12, 2024, 10:56 AM
 */

#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader
{
public:
    unsigned int ID;
    
    Shader(const char* vertexFile, const char* fragmentFile) {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        
        try {
            vShaderFile.open(vertexFile);
            fShaderFile.open(fragmentFile);
            std::stringstream vShaderStream, fShaderStream;
            
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            
            vShaderFile.close();
            fShaderFile.close();
            
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        
        catch (std::ifstream::failure e) {
            std::cout << e.what() << std::endl;
        }
        
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];
        
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "(Vertex) " << infoLog;
        }
        
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "(Fragment) " << infoLog;
        }
        
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "(Program) " << infoLog;
        }
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    
    void use() {
        glUseProgram(ID);
    }
    
    void setBool(const char* name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name), (int)value);
    }
    void setInt(const char* name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name), value);
    }
    void setFloat(const char* name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name), value);
    }
    void setMat4(const char* name, glm::mat4 value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
    }
    void setVec3(const char* name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(ID, name), x, y, z);
    }
    void setVec3(const char* name, glm::vec3 value) {
        glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
    }
    void setVec4(const char* name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
    }
    void setVec4(const char* name, glm::vec4 value) {
        glUniform4f(glGetUniformLocation(ID, name), value.x, value.y, value.z, value.w);
    }
};

#endif /* SHADER_H */

