#include<Model.h>
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
    vector<GLuint> texIndices;
    //Mesh *mesh=(Mesh *)malloc(sizeof(Mesh));
    bool hasTexture=false;                       //if the model contains texture coordinates
    bool hasNormal=false;                        //if the model contains normal data
    // bool isFirstMesh=true;
    while(getline(ifs,line))
    {
        if(line.substr(0,6)=="mtllib")              //declaration of reference material library
        {
            loadMaterial(line.substr(7));
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
            vector<Vertex> vec;
            Vertex vertex;
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
            Vertex first=vec.front();           //the first vertex in vector
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
        else if(line[0]=='g')                   //another group
        {
            Mesh mesh(faces,texIndices);
            if(!hasNormal)
                GenNormals(mesh);
            this->meshes.push_back(mesh);       //store this mesh
            faces.clear();
            texIndices.clear();
            hasTexture=true;                    //set hasTexture
            hasNormal=true;                     //set hasNormal
        }
        else if(line.substr(0,6)=="usemtl")     //declaration of using material
        {
            string mtlName=line.substr(7);
            //scan through the loaded textures to find the corresponding textures belonging to the material
            for(int i=0;i<textures.size();i++)
            {
                Texture tex=textures[i];
                if(tex.mtlName==mtlName)
                {
                    texIndices.push_back(i);        //store the texture index
                }
            }
        }
        else if(line[0]=='#')                   //comments
        {
            //skip the line
        }
        
    }


    //read obj file ended

    Mesh mesh(faces,texIndices);
    if (!hasNormal)
        GenNormals(mesh);
    this->meshes.push_back(mesh); //store this mesh
    faces.clear();
    texIndices.clear();

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
    Vertex *v1, *v2, *v3;
    glm::vec3 temp_vec1, temp_vec2, vec;
    Face face;
    //iterate through each face
    for (int i = 0; i < mesh.faces.size(); i++)
    {
        face = mesh.faces[i];
        v1 = &face.vertex1;
        v2 = &face.vertex2;
        v3 = &face.vertex3;
        temp_vec1 = this->verCoords[v1->verCoordIndex] - this->verCoords[v1->verCoordIndex];
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
    vector<Texture> textures;
    std::ifstream ifs(materialPath);
    std::string line;
    Texture tex;
    while(getline(ifs,line))
    {
        string ltype;
        string mtlName;
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
            for(int i=0;i<textures.size();i++)
            {
                if(texName==textures[i].name)
                {
                    texID=textures[i].id;
                    skip=true;
                    break;
                }

            }
            if(!skip)
                texID=TextureFromFile(texName,this->directory);        //load new texture

            tex.id=texID;
            tex.name=texName;
            tex.type="map_Kd";
            tex.mtlName=mtlName;
            textures.push_back(tex);
        }
        else if(ltype=="map_Ka")                                    //an ambient texture from file                 
        {
            string texName;
            s>>texName;
            bool skip=false;
            Texture tex;
            GLuint texID;
            //search textures to see if this texture has already been loaded
            for(int i=0;i<textures.size();i++)
            {
                if(texName==textures[i].name)
                {
                    texID=textures[i].id;
                    skip=true;
                    break;
                }

            }
            if(!skip)
                texID=TextureFromFile(texName,this->directory);        //load new texture

            tex.id=texID;
            tex.name=texName;
            tex.type="map_Ka";
            tex.mtlName=mtlName;
            textures.push_back(tex);
        }
        else if(line[0]=='#')                   //comment line
        {

        }
    }
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
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}