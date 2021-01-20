#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>
using namespace std;

//each VertexWithIndex contains indices of its coordinate, normal and texture coordinate
//coordinate, normal and coordinate data are stored in struct Model after LoadModel()
//Model calls copyMeshData() to copy corresponding data to the mesh using these indices information
struct VertexWithIndex {
    GLuint verCoordIndex;       //vertex coordinate index
    GLuint norIndex;            // normal index
    GLuint texCoordIndex;       // texture coordinate index

};

struct Face{                    //store three vertex indices of the face
    VertexWithIndex vertex1;
    VertexWithIndex vertex2;
    VertexWithIndex vertex3;
};


struct Texture {
    GLuint id;
    string type;
    string name;
    string mtlName;
};


//vertex data
struct Vertex {
    // position
    glm::vec3 verCoord;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 texCoord;
};

//store material data of the mesh
struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    string mtlName;
};


class Mesh {
public:
    vector<Face>    faces;                  //faces of the mesh
    
    // mesh Data
    vector<Vertex>       vertices;          //vertices data of the mesh
    vector<unsigned int> indices;           //indices of the mesh
    vector<Texture>      textures;          //textures of the mesh
    Material      material;                 //material information of the mesh

    unsigned int VAO;

    //constructor
    Mesh(vector<Face> faces, vector<Texture> textures, Material material)
    {
        this->faces=faces;
        //this->texIndices=texIndices;
        this->textures=textures;
        this->material.ambient= material.ambient;
        this->material.diffuse = material.diffuse;
        this->material.specular = material.specular;
        this->material.shininess = material.shininess;
    }

    void Draw(Shader &shader);              // render the mesh
    void SetupMesh(void);                   //initializes all the buffer objects/arrays

private:
    // render data 
    unsigned int VBO, EBO;

};
#endif