#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <iostream>
#include "Mesh.h"
#include "Shader.h"

class Model
{   
    std::vector<Texture> textures;
    std::vector<Mesh> meshes;

    inline Model(const std::string &path){
        loadMesh(path);
    }
    void Draw(Shader & shader);
    
private:
    Shader* shader;

    void loadMesh(std::string path);

    /* You can add more functions here freely */
};

#endif