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
        Model(char *path) {
            loadModel(path);
        }

        void Draw(Shader &shader) {
            for (unsigned int i = 0; i < meshes.size(); i++) {
                meshes[i].Draw(shader);
            }
        }

    private:
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
#endif