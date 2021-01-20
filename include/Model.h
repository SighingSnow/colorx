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


#include "Mesh.h"


//a model loaded from .obj file and the corresponding .mtl file
class Model
{   

public:
    //model data
    vector<glm::vec3>   verCoords;          //vertex coordinates data
    vector<glm::vec2>   texCoords;          //texture coordinates data
    vector<glm::vec3>   normals;            //normal data


    vector<Texture>     texturesLoaded;     //textures data
    vector<Material>    materialsLoaded;    //material data
    std::vector<Mesh> meshes;               //meshes

    std::string directory;                  //model directory
    std::string modelName;                  //model name

    Shader* shader;                         //shader

    glm::vec3 position;
    glm::vec3 scale;

    //constructor
    Model(const std::string path,glm::vec3 position, glm::vec3 scale){
        modelName=path.substr(path.rfind('/')+1);
        directory=path.substr(0,path.rfind('/'));
        this->position = position;
        this->scale = scale;
        loadModel(path);
        CopyMeshData();
    }
    
    void Draw(Shader &shader);                  //render the model
    
private:

    void loadModel(std::string modelPath);                          //load a model from a .obj file
    void GenNormals(Mesh &mesh);                                    //generate normals for each vertex of the mesh
    void loadMaterial(string materialPath);                         //load material from .mtl file                      
    GLuint TextureFromFile(string textureName, string directory);   //load a texture from file
    void CopyMeshData(void);                                        //copy data to each mesh after loading the .obj file

};

#endif