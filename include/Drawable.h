#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <Shader.h>
#include <stb_image.h>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 texCoord;
};

struct Texture {
    unsigned int ID;
    std::string type;
    std::string name;
};

unsigned int TextureFromFile(const char *fileName, const char *directory) {
    std::string path = std::string(fileName) + std::string(directory);
    
    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    int format;
    if (nrChannels == 1) {
        format = GL_RED;
    }
    else if (nrChannels == 3) {
        format = GL_RGB;
    }
    else if (nrChannels == 4) {
        format = GL_RGBA;
    }
    else {
        printf("Could not load texture %s since it has an unusual amount of channels", path);
        return tex;
    }

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }

    else {
        printf("Could not load texture %s", path);
    }

    return tex;
}

class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, std::vector<Texture> texes) {
            this->vertices = verts;
            this->indices = inds;
            this->textures = texes;

            setupMesh();
        }

        void Draw (Shader &shader) {
            unsigned int numDiffuse = 1;
            unsigned int numSpecular = 1;

            for (int i = 0; i < textures.size(); i++) {
                glActiveTexture(GL_TEXTURE + i);
                std::string num;
                std::string name;

                if (name == "texture_diffuse") {
                    num = std::to_string(numDiffuse++);
                }
                else if (name == "texture_specular") {
                    num = std::to_string(numSpecular++);
                }

                shader.setInt(("material." + name + num).c_str(), i);
                glBindTexture(GL_TEXTURE_2D, textures[i].ID);
            }

            glActiveTexture(GL_TEXTURE0);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

    private:
        unsigned int VAO, VBO, EBO;

        void setupMesh() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

            glBindVertexArray(0);
        }
};
#endif