#include "../include/Model.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"

#include<fstream>
#include<iostream>

#define NOT_A_INDEX -1


//render the model
void Model::Draw(Shader &shader)
{
    //loops over each of the meshes to call their respective Draw function
    for(unsigned int i=0;i<meshes.size();i++)
        meshes[i].Draw(shader);
}

//load a model from a .obj file
void Model::loadModel(std::string modelPath)
{

    std::ifstream ifs(modelPath);
    std::string line;
    vector<Face>   faces;
    vector<Texture> textures;
    Material material;
    //Mesh *mesh=(Mesh *)malloc(sizeof(Mesh));
    bool hasTexture=false;                       //if the model contains texture coordinates
    bool hasNormal=false;                        //if the model contains normal data
    bool isFirstMesh=true;
    while(getline(ifs,line))
    {
        if(line.substr(0,6)=="mtllib")              //declaration of reference material library
        {
            loadMaterial(this->directory+"/"+line.substr(7));
        }
        else if(line.substr(0,2)=="vt")              //Vertex texture coordinates
        {
            hasTexture=true;
            std::istringstream s(line.substr(2));
            glm::vec2 texcoord;
            s >> texcoord.x;
            s >> texcoord.y;
            this->texCoords.push_back(texcoord);
        }
        else if(line.substr(0,2)=="vn")         //vertex normal
        {
            hasNormal=true;
            std::istringstream s(line.substr(2));
            glm::vec3 normal;
            s>>normal.x;
            s>>normal.y;
            s>>normal.z;
            this->normals.push_back(normal);
        }
        else if(line.substr(0,1)=="v")          //vertex coordinates
        {
            std::istringstream s(line.substr(1));
            glm::vec3 vercoord;
            s>>vercoord.x;
            s>>vercoord.y;
            s>>vercoord.z;
            this->verCoords.push_back(vercoord);
        }
        else if(line.substr(0,1)=="f")          //face data
        {
            vector<VertexWithIndex> vec;
            VertexWithIndex vertex;
            string vertexStr;
            std::istringstream s(line.substr(1));

            while(s>>vertexStr)
            {
                GLuint verCoordIndex;
                GLuint texCoordIndex;
                GLuint norIndex;
               
                // if(vertexStr.rfind('/')==vertexStr.find('/'))
                // {
                //     hasNormal=false;
                // }
                if(hasTexture&&hasNormal)
                {
                    verCoordIndex=stoi(vertexStr.substr(0,vertexStr.find('/')))-1;        //vertex coordinates index
                    texCoordIndex = stoi(vertexStr.substr(vertexStr.find('/') + 1, vertexStr.rfind('/') - vertexStr.find('/'))) - 1; //texture coordinates index
                    norIndex = stoi(vertexStr.substr(vertexStr.rfind('/') + 1)) - 1; //normal index
                    
                }
                else if(hasTexture&&(!hasNormal))
                {
                    verCoordIndex=stoi(vertexStr.substr(0,vertexStr.find('/')))-1;        //vertex coordinates index
                    texCoordIndex=stoi(vertexStr.substr(vertexStr.find('/') + 1)) - 1;   //texture coordinates index
                    norIndex=NOT_A_INDEX;
                }
                else if((!hasTexture)&&hasNormal)
                {
                    verCoordIndex=stoi(vertexStr.substr(0,vertexStr.find('/')))-1;        //vertex coordinates index
                    texCoordIndex=NOT_A_INDEX;
                    norIndex=stoi(vertexStr.substr(vertexStr.rfind('/')+1))-1;      //normal index
                }
                else
                {
                    verCoordIndex = stoi(vertexStr) - 1;            //texture coordinates index
                    texCoordIndex=NOT_A_INDEX;
                    norIndex=NOT_A_INDEX;
                }
                vertex.verCoordIndex=verCoordIndex;
                vertex.texCoordIndex = texCoordIndex;
                vertex.norIndex = norIndex;
                vec.push_back(vertex);
            }

            //Triangulate the face
            int vertexNum=vec.size();           //vertex number of the face
            int triangleNum=vertexNum-2;        //number of triangles in this face
            VertexWithIndex first=vec.front();           //the first vertex in vector
            vec.erase(vec.begin());
            for(int i=0;i<triangleNum;i++)
            {
                Face face;
                face.vertex1=vec.front();
                vec.erase(vec.begin());
                face.vertex2=vec.front();
                face.vertex3=first;
                faces.push_back(face);     //store the triangle face into the mesh
            }

        }
        //else if(line[0]=='g')                   //another group
        //{
        //    //store last mesh
        //    if (!isFirstMesh)
        //    {
        //        Mesh mesh(faces, textures);
        //        if (!hasNormal)
        //            GenNormals(mesh);
        //        this->meshes.push_back(mesh);       //store this mesh
        //        faces.clear();
        //        textures.clear();
        //        hasTexture = false;                    //set hasTexture
        //        hasNormal = false;                     //set hasNormal
        //    }
        //    else
        //    {
        //        isFirstMesh = false;
        //    }
        //}
        //else if (line[0] == 'o')                   //another object
        //{
        //    //store last mesh
        //    if (!isFirstMesh)
        //    {
        //        Mesh mesh(faces, textures);
        //        if (!hasNormal)
        //            GenNormals(mesh);
        //        this->meshes.push_back(mesh);       //store this mesh
        //        faces.clear();
        //        textures.clear();
        //        hasTexture = false;                    //set hasTexture
        //        hasNormal = false;                     //set hasNormal
        //    }
        //    else
        //    {
        //        isFirstMesh = false;
        //    }
        //}
        else if(line.substr(0,6)=="usemtl")     //declaration of using material
        {
            //store last mesh
            if (!isFirstMesh)
            {
                Mesh mesh(faces, textures,material);
                if (!hasNormal)
                    GenNormals(mesh);
                this->meshes.push_back(mesh);       //store this mesh
                faces.clear();
                textures.clear();
                //hasTexture = false;                    //set hasTexture
                //hasNormal = false;                     //set hasNormal
            }
            else
            {
                isFirstMesh = false;
            }

            string mtlName=line.substr(7);
            //scan through the loaded textures to find the corresponding textures belonging to the material
            for(int i=0;i<this->texturesLoaded.size();i++)
            {
                Texture tex=this->texturesLoaded[i];
                if(tex.mtlName==mtlName)
                {
                    textures.push_back(tex);        //store the texture index
                }
            }
            for (int i = 0; i < this->materialsLoaded.size(); i++)
            {
                Material mat = this->materialsLoaded[i];
                if (mat.mtlName == mtlName)
                {
                    material.ambient= mat.ambient;
                    material.diffuse = mat.diffuse;
                    material.specular = mat.specular;
                    material.shininess = mat.shininess;
                    break;
                }
            }
        }
        else if(line[0]=='#')                   //comments
        {
            //skip the line
        }
        
    }


    //read obj file ended

    Mesh mesh(faces,textures,material);
    if (!hasNormal)
        GenNormals(mesh);
    this->meshes.push_back(mesh); //store this mesh
    faces.clear();
    textures.clear();

    //normalize all normals
    for(int i=0;i<this->normals.size();i++)
    {
        this->normals[i]=glm::normalize(this->normals[i]);
    }

}


//generate normal for each vertex of the mesh
void Model::GenNormals(Mesh &mesh)
{
    GLuint *map = (GLuint *)malloc(sizeof(GLuint) * (this->verCoords.size())); //map from vertex index to normal index
    for (int i = 0; i < this->verCoords.size(); i++)                           //initialize map
    {
        map[i] = NOT_A_INDEX;
    }
    VertexWithIndex *v1, *v2, *v3;
    glm::vec3 temp_vec1, temp_vec2, vec;
    Face *face;
    //iterate through each face
    for (int i = 0; i < mesh.faces.size(); i++)
    {
        face = &(mesh.faces[i]);
        v1 = &(face->vertex1);
        v2 = &(face->vertex2);
        v3 = &(face->vertex3);
        temp_vec1 = this->verCoords[v1->verCoordIndex] - this->verCoords[v2->verCoordIndex];
        temp_vec2 = this->verCoords[v2->verCoordIndex] - this->verCoords[v3->verCoordIndex];
        vec = glm::normalize(glm::cross(temp_vec1, temp_vec2)); //Cross product of two vectors, normalization
        //add the vec to v1's normal
        if (map[v1->verCoordIndex] == NOT_A_INDEX) //this vertex normal hasn't been calculated
        {
            map[v1->verCoordIndex] = this->normals.size();
            v1->norIndex = this->normals.size();
            this->normals.push_back(vec);
        }
        else
        {
            this->normals[map[v1->verCoordIndex]] += vec;
        }
        //add the vec to v2's normal
        if (map[v2->verCoordIndex] == NOT_A_INDEX) //this vertex normal hasn't been calculated
        {
            map[v2->verCoordIndex] = this->normals.size();
            v2->norIndex = this->normals.size();
            this->normals.push_back(vec);
        }
        else
        {
            this->normals[map[v2->verCoordIndex]] += vec;
        }
        //add the vec to v3's normal
        if (map[v3->verCoordIndex] == NOT_A_INDEX) //this vertex normal hasn't been calculated
        {
            map[v3->verCoordIndex] = this->normals.size();
            v3->norIndex = this->normals.size();
            this->normals.push_back(vec);
        }
        else
        {
            this->normals[map[v3->verCoordIndex]] += vec;
        }
    }

    delete[] map;
}


//load material from .mtl file
void Model::loadMaterial(string materialPath)
{
    vector<Texture> texturesLoaded;
    vector<Material> materialsLoaded;
    std::ifstream ifs(materialPath);
    std::string line;
    Texture tex;
    Material mat;
    string mtlName;
    while(getline(ifs,line))
    {
        string ltype;
        std::istringstream s(line);
        s>>ltype;
        if(ltype=="newmtl")             //a new material
        {
            s>>mtlName;
        }
        else if(ltype=="map_Kd")        //an diffuse texture from file 
        {
            string texName;
            s>>texName;
            bool skip=false;
            Texture tex;
            GLuint texID;
            //search textures to see if this texture has already been loaded
            for(int i=0;i<texturesLoaded.size();i++)
            {
                if(texName==texturesLoaded[i].name)
                {
                    texID=texturesLoaded[i].id;
                    skip=true;
                    break;
                }

            }
            if(!skip)
                texID=TextureFromFile(texName,this->directory);        //load new texture

            tex.id=texID;
            tex.name=texName;
            tex.type="texture_diffuse";
            tex.mtlName=mtlName;
            texturesLoaded.push_back(tex);
        }
        else if(ltype=="map_Ks")                                    //an ambient texture from file                 
        {
            string texName;
            s>>texName;
            bool skip=false;
            Texture tex;
            GLuint texID;
            //search textures to see if this texture has already been loaded
            for(int i=0;i<texturesLoaded.size();i++)
            {
                if(texName==texturesLoaded[i].name)
                {
                    texID=texturesLoaded[i].id;
                    skip=true;
                    break;
                }

            }
            if(!skip)
                texID=TextureFromFile(texName,this->directory);        //load new texture

            tex.id=texID;
            tex.name=texName;
            tex.type="texture_specular";
            tex.mtlName=mtlName;
            texturesLoaded.push_back(tex);
        }
        else if(ltype=="map_Bump")                                    //an ambient texture from file                 
        {
            string texName;
            s>>texName;
            bool skip=false;
            Texture tex;
            GLuint texID;
            //search textures to see if this texture has already been loaded
            for(int i=0;i<texturesLoaded.size();i++)
            {
                if(texName==texturesLoaded[i].name)
                {
                    texID=texturesLoaded[i].id;
                    skip=true;
                    break;
                }

            }
            if(!skip)
                texID=TextureFromFile(texName,this->directory);        //load new texture

            tex.id=texID;
            tex.name=texName;
            tex.type="texture_height";
            tex.mtlName=mtlName;
            texturesLoaded.push_back(tex);
        }
        else if(ltype=="Ka")
        {
            float x,y,z;
            s>>x;
            s>>y;
            s>>z;
            mat.ambient.x=x;
            mat.ambient.y=y;
            mat.ambient.z=z;
        }
        else if(ltype=="Kd")
        {
            float x,y,z;
            s>>x;
            s>>y;
            s>>z;
            mat.diffuse.x=x;
            mat.diffuse.y=y;
            mat.diffuse.z=z;
        }
        else if(ltype=="Ks")
        {
            float x,y,z;
            s>>x;
            s>>y;
            s>>z;
            mat.specular.x=x;
            mat.specular.y=y;
            mat.specular.z=z;

            mat.mtlName=mtlName;
            materialsLoaded.push_back(mat);
        }
        
        else if(line[0]=='#')                   //comment line
        {

        }
    }

    this->texturesLoaded=texturesLoaded;
    this->materialsLoaded=materialsLoaded;
}



//load a texture from file
GLuint Model::TextureFromFile(string textureName, string directory)
{
    string path= directory + '/' + textureName;

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Model$ Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


void Model::CopyMeshData(void)
{
    GLuint* map = (GLuint*)malloc(sizeof(GLuint) * (this->verCoords.size()));
    //loop through each mesh in the Model
    for(int i=0;i<this->meshes.size();i++)
    {
        vector<Vertex>       vertices;      //store mesh's vertex data
        vector<unsigned int> indices;       //store mesh's vertex indices
        unsigned int index=0;
        //map from vertex coordinate index to vertex index in a mesh
        for (int i = 0; i < this->verCoords.size(); i++)                           //initialize map
        {
            map[i] = NOT_A_INDEX;
        }

        Mesh mesh=this->meshes[i];      //get one mesh
        for(int j=0;j<mesh.faces.size();j++)                //process each face
        {
            Face face=mesh.faces[j];    //get one face
            //process vertex1 of the face
            VertexWithIndex vertexWithIndex1=face.vertex1;
            if(map[vertexWithIndex1.verCoordIndex]==NOT_A_INDEX)        //the vertex data hasn't yet been stored
            {
                Vertex vertex;
                //get vertex data
                vertex.verCoord=this->verCoords[vertexWithIndex1.verCoordIndex];
                vertex.normal=this->normals[vertexWithIndex1.norIndex];
                if(this->texCoords.size()>vertexWithIndex1.texCoordIndex)
                vertex.texCoord=this->texCoords[vertexWithIndex1.texCoordIndex];
                vertices.push_back(vertex);         //store vertex data
                
                map[vertexWithIndex1.verCoordIndex]=index;      //update map
                indices.push_back(index++);         //store vertex index
            }
            else                    //already stored the vertex data in the mesh
            {
                indices.push_back(map[vertexWithIndex1.verCoordIndex]); //store vertex index
            }

            //process vertex2 of the face
            VertexWithIndex vertexWithIndex2=face.vertex2;
            if(map[vertexWithIndex2.verCoordIndex]==NOT_A_INDEX)        //the vertex data hasn't yet been stored
            {
                Vertex vertex;
                //get vertex data
                vertex.verCoord=this->verCoords[vertexWithIndex2.verCoordIndex];
                vertex.normal=this->normals[vertexWithIndex2.norIndex];
                if(this->texCoords.size()>vertexWithIndex2.texCoordIndex)
                vertex.texCoord=this->texCoords[vertexWithIndex2.texCoordIndex];
                vertices.push_back(vertex);         //store vertex data
                
                map[vertexWithIndex2.verCoordIndex]=index;      //update map
                indices.push_back(index++);         //store vertex index
            }
            else                    //already stored the vertex data in the mesh
            {
                indices.push_back(map[vertexWithIndex2.verCoordIndex]); //store vertex index
            }

            //process vertex3 of the face
            VertexWithIndex vertexWithIndex3=face.vertex3;
            if(map[vertexWithIndex3.verCoordIndex]==NOT_A_INDEX)        //the vertex data hasn't yet been stored
            {
                Vertex vertex;
                //get vertex data
                vertex.verCoord=this->verCoords[vertexWithIndex3.verCoordIndex];
                vertex.normal=this->normals[vertexWithIndex3.norIndex];
                if(this->texCoords.size()>vertexWithIndex3.texCoordIndex)
                vertex.texCoord=this->texCoords[vertexWithIndex3.texCoordIndex];
                vertices.push_back(vertex);         //store vertex data
                
                map[vertexWithIndex3.verCoordIndex]=index;      //update map
                indices.push_back(index++);         //store vertex index
            }
            else                    //already stored the vertex data in the mesh
            {
                indices.push_back(map[vertexWithIndex3.verCoordIndex]); //store vertex index
            }


        }

        this->meshes[i].vertices=vertices;
        this->meshes[i].indices=indices;

        this->meshes[i].SetupMesh();
        
    }

    this->verCoords.clear();
    this->normals.clear();
    this->texCoords.clear();
    delete[] map;



}

