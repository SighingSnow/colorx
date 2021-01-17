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
//coordinate, normal and coordinate data are stored in struct Model 
struct VertexWithIndex {
    GLuint verCoordIndex;       //vertex coordinate index
    GLuint norIndex;            // normal index
    GLuint texCoordIndex;       // texture coordinate index

};

struct Face{                    //store three vertices of the face
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


//real vertex data
struct Vertex {
    // position
    glm::vec3 verCoord;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 texCoord;
};



class Mesh {
public:
    // mesh Data
    vector<Face>    faces;              //faces of the mesh

    //vector<GLuint>  texIndices;         //indices of textures used in this mesh
                                        //texture data are stored in struct Model
    
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;

    unsigned int VAO;

    //constructor
    Mesh(vector<Face> faces, vector<Texture> textures)
    {
        this->faces=faces;
        //this->texIndices=texIndices;
        this->textures=textures;
    }



    // render the mesh
    void Draw(Shader &shader);
    void SetupMesh(void);

private:
    // render data 
    unsigned int VBO, EBO;

};
#endif