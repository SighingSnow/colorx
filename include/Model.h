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


//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
#include "Mesh.h"


class Model
{   

public:
    //model data
    vector<glm::vec3>   verCoords;          //vertex coordinates data
    vector<glm::vec2>   texCoords;          //texture coordinates data
    vector<glm::vec3>   normals;            //normal data


    vector<Texture>     texturesLoaded;           //textures data
    vector<Material>    materialsLoaded;
    std::vector<Mesh> meshes;               //meshes

    std::string directory;                  //model directory
    std::string modelName;                  //model name

    Shader* shader;

    glm::vec3 position;
    glm::vec3 scale;

    //constructor
    Model(const std::string path,glm::vec3 position, glm::vec3 scale){
        //printf("In Model constructor\n");
        modelName=path.substr(path.rfind('/')+1);
        directory=path.substr(0,path.rfind('/'));
        this->position = position;
        this->scale = scale;
        loadModel(path);
        CopyMeshData();
    }
    void Draw(Shader &shader);
    
private:


    void loadModel(std::string modelPath);
    void GenNormals(Mesh &mesh);
    void loadMaterial(string materialPath);
    GLuint TextureFromFile(string textureName, string directory);
    void CopyMeshData(void);

};

#endif