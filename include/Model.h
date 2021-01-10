#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

struct Texture {
    GLuint id;
    string type;
    string name;
    string mtlName;
};

struct Material{
    vector<Texture> diffuseMaps;
    vector<Texture> specularMaps;
    vector<Texture> normalMaps;
    vector<Texture> heightMaps;

};


class Model
{   

public:
    //model data
    vector<glm::vec3>   verCoords;          //vertex coordinates data
    vector<glm::vec2>   texCoords;          //texture coordinates data
    vector<glm::vec3>   normals;            //normal data
    vector<Texture>     textures;           //textures data
    std::vector<Mesh> meshes;               //meshes

    std::string directory;                  //model directory
    std::string modelName;                  //model name

    Shader* shader;

    //constructor
    Model(const std::string path){
        modelName=path.substr(path.rfind('/')+1);
        directory=path.substr(0,path.rfind('/'));
        loadModel(path);
    }
    void Draw(Shader &shader);
    
private:


    void loadModel(std::string modelPath);
    void GenNormals(Mesh &mesh);
    void loadMaterial(string materialPath);
    GLuint TextureFromFile(string textureName, string directory);

};

#endif