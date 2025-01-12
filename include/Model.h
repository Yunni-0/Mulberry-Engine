#ifndef MODEL_H
#define MODEL_H

#include <Shader.h>
#include <Drawable.h>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
    public:
        Model(const char *path, glm::vec3 pos, glm::vec4 rot, glm::vec3 scale) {
            modelMat = glm::translate(modelMat, pos);
            modelMat = glm::rotate(modelMat, rot.w, glm::vec3(rot.x, rot.y, rot.z));
            modelMat = glm::scale(modelMat, scale);
            loadModel(path);
        }

        void Draw(Shader &shader) {
            for (unsigned int i = 0; i < meshes.size(); i++) {
                meshes[i].Draw(shader, modelMat);
            }
        }

    private:
        glm::mat4 modelMat = glm::mat4(1.0f);
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(std::string path) {
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                printf("(Model) Error: %s", importer.GetErrorString());
                return;
            }

            directory = path.substr(0, path.find_last_of('/'));

            processNode(scene->mRootNode, scene);
        }

        void processNode(aiNode *node, const aiScene *scene) {
            for (unsigned int i = 0; i < node->mNumMeshes; i++) {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
            }

            for (unsigned int i = 0; i < node->mNumChildren; i++) {
                processNode(node->mChildren[i], scene);
            }
        }

        Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<Texture> textures;

            for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
                Vertex vertex;

                vertex.pos = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
                vertex.norm = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
                
                if (mesh->mTextureCoords[0]) {
                    vertex.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
                }
                else {
                    vertex.texCoord = glm::vec2(0.0f);
                }

                vertices.push_back(vertex);
            }

            for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
                for (unsigned int n = 0; n < mesh->mFaces[i].mNumIndices; n++) {
                    indices.push_back(mesh->mFaces[i].mIndices[n]);
                }
            }

            if (mesh->mMaterialIndex >= 0) {
                aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
                std::vector<Texture> diffuseMaps = loadMaterialTextures(scene, mat, aiTextureType_DIFFUSE, "texture_diffuse");
                textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end()); 
                std::vector<Texture> specularMaps = loadMaterialTextures(scene, mat, aiTextureType_SPECULAR, "texture_specular");
                textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            }

            return Mesh(vertices, indices, textures);
        }

        std::vector<Texture> loadMaterialTextures(const aiScene *scene, aiMaterial *mat, aiTextureType type, std::string typeName) {
            std::vector<Texture> textures;
            
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
                Texture texture;
                aiString name;
                mat->GetTexture(type, i, &name);

                bool skip = false;
                for (unsigned int n = 0; n < textures.size(); n++) {
                    if (strcmp(textures[n].name.data(), name.C_Str()) == 0) {
                        textures.push_back(textures[n]);
                        skip = true;
                        break;
                        
                    }
                }

                if (!skip) {
                    texture.ID = TextureFromFile(scene, name.C_Str(), directory.c_str());
                    texture.type = typeName;
                    texture.name = name.C_Str();
                    textures.push_back(texture);
                }
            }

            return textures;
        }
};
#endif