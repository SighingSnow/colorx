/*
 * Function:
 * Camera / Light / Mesh is all in here
 */
#pragma once
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <iostream>
#include <vector>
#include "Light.h"
#include "Camera.h"
#include "Model.h"
#include "Core.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <ctime>


enum _TYPE_
{
	_MESH_SCENE_=0,
	_CUBE_,
	_SPHERE_,
	_CYLINDER_,
	_CONE_,
	_FRUSTUM_,
	_PYRAMID_
};

typedef enum _TYPE_ TYPE;

struct _TRANSFORM_ATTRIBUTE_
{
	glm::vec3 Position;
	glm::vec3 RotAxis;
	glm::vec3 Scale;
	glm::vec3 Color;
	float RotAngle;
	// The number of faces is no less than 3
	int FaceNum;
	/*
	 *	Ratio refers to the ratio between the radius of the top and the bottom circle.
	 *	That is, Ratio = Radius(Top) / Radius(Bottom).
	 *	Its domain is [0, 1]
	 *	If it is 0, that the frustum becomes a pyramid
	 */ 
	float Ratio;
	bool pick;
	bool isAlive;
};

typedef struct _TRANSFORM_ATTRIBUTE_ transAttr;

class SceneNode
{
public:
	TYPE type;
	transAttr NodeAttr;
	bool texture = false;
	unsigned int textureID;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;


	inline SceneNode(TYPE Type, bool Texture = false, unsigned int TextureID = 0)
	{
		type = Type;
		texture = Texture;
		textureID = TextureID;
		NodeAttr.Position	= glm::vec3(0.0);
		NodeAttr.RotAngle	= 0;
		NodeAttr.RotAxis	= glm::vec3(1.0, 0.0, 0.0);
		NodeAttr.Scale		= glm::vec3(1.0);
		NodeAttr.Color		= glm::vec3(1.0);
		NodeAttr.FaceNum	= 3;
		NodeAttr.Ratio		= 1;
		NodeAttr.pick 		= false;
		NodeAttr.isAlive 	= true;
		setUpSceneNode(type);
	}

	inline SceneNode(transAttr transform, TYPE Type, bool Texture = false, unsigned int TextureID = 0)
	{
		type				= Type;
		texture				= Texture;
		textureID			= TextureID;
		NodeAttr.Position	= transform.Position;
		NodeAttr.RotAngle	= glm::radians(transform.RotAngle);
		NodeAttr.RotAxis	= transform.RotAxis;
		NodeAttr.Scale		= transform.Scale;
		NodeAttr.Color		= transform.Color;
		NodeAttr.pick 		= transform.pick;
		NodeAttr.isAlive	= transform.isAlive;

		if		(Type == _FRUSTUM_ 	|| Type == _PYRAMID_)		NodeAttr.FaceNum = transform.FaceNum;
		else if (Type == _CONE_		|| Type == _CYLINDER_)		NodeAttr.FaceNum = 100;
		
		if		(Type == _FRUSTUM_ 	|| Type == _CYLINDER_)		NodeAttr.Ratio = transform.Ratio;
		else if (Type == _CONE_		|| Type == _PYRAMID_)		NodeAttr.Ratio = 0;
		
		setUpSceneNode(type);
	}

	void draw(Shader &shader);

	bool ifCollision(glm::vec3 nxtPos);

	~SceneNode(){
		//delete this;
	};
private:
	unsigned int VAO,VBO,EBO;

	void GenStdCube();
	void GenStdSphere();
	void GenStdFrustum();

	void setUpSceneNode(TYPE type){
		switch(type){
			case _CUBE_: GenStdCube(); break;
			case _SPHERE_: GenStdSphere(); break;
			case _CYLINDER_:
			case _CONE_:
			case _FRUSTUM_:
			case _PYRAMID_:	GenStdFrustum();break;
			default: std::cout<<"Current type isn't supported.";break;
		}
		
		glGenVertexArrays(1,&VAO);
		glGenBuffers(1,&VBO);
		glGenBuffers(1,&EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(float)*vertices.size(),&vertices[0], GL_STATIC_DRAW);  
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
        
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
		
		glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
		
		glBindVertexArray(0);
	}
};

class Skybox
{
public:
	unsigned int textureID;
	inline Skybox(unsigned int TextureID){
		textureID = TextureID;
		setUpSkybox();
	}

	void draw();

	Skybox(){};
	~Skybox(){};

private:
	unsigned int VAO,VBO;
	float skyboxVertices[108] = {
		// positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f};
	
	void setUpSkybox()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindVertexArray(0);
	}
};


class SceneManager
{
public:
	GLFWwindow *window;
	Camera* camera;
	Shader* commonShader;
	Shader* meshShader;
	Shader* skyboxShader;
	Skybox ourSkybox;
	std::vector<Model> meshNodes;
	std::vector<Light> lights;
	std::vector<SceneNode> commonNodes;

	bool wire = false;

	unsigned int StoneTex = loadTexture("../resource/texture_image/stone.png");
	unsigned int ObsidianTex = loadTexture("../resource/texture_image/obsidian.png");
	unsigned int WoodTex = loadTexture("../resource/texture_image/wood.png");
	unsigned int BrickTex = loadTexture("../resource/texture_image/brick.png");

	unsigned int skyboxTex = loadCubemap(Faces);

	inline SceneManager(GLFWwindow* mywindow){
		window = mywindow;
		commonShader = new Shader(ordinary_type);
		meshShader = new Shader(mesh_type);
		skyboxShader = new Shader(skybox_type);
		camera = new Camera(glm::vec3(0.0f,0.0f,3.0f));
		ourSkybox = *new Skybox(skyboxTex);
	};

	void addMeshSceneNode(SceneManager *smgr, const char* path );

	void addCubeNode(SceneManager *smgr, bool Texture = false, unsigned int TextureID = 0);
	void addCubeNode(SceneManager *smgr, transAttr transform, bool Texture = false, unsigned int TextureID = 0);

	void addSphereNode(SceneManager *smgr, bool Texture = false, unsigned int TextureID = 0);
	void addSphereNode(SceneManager *smgr, transAttr transform, bool Texture = false, unsigned int TextureID = 0);

	void addCylinderNode(SceneManager *smgr, bool Texture = false, unsigned int TextureID = 0);
	void addCylinderNode(SceneManager *smgr, transAttr transform, bool Texture = false, unsigned int TextureID = 0);

	void addConeNode(SceneManager *smgr, bool Texture = false, unsigned int TextureID = 0);
	void addConeNode(SceneManager *smgr, transAttr transform, bool Texture = false, unsigned int TextureID = 0);

	void addFrustumNode(SceneManager *smgr, bool Texture = false, unsigned int TextureID = 0);
	void addFrustumNode(SceneManager *smgr, transAttr transform, bool Texture = false, unsigned int TextureID = 0);

	void addPyramidNode(SceneManager *smgr, bool Texture = false, unsigned int TextureID = 0);
	void addPyramidNode(SceneManager *smgr, transAttr transform, bool Texture = false, unsigned int TextureID = 0);

	/* draw all the meshNodes and Nodes */
	void drawAll();

	/* auto save to current dir */
	void prtScreen();

	bool intersect(glm::vec3 camPos,glm::vec3 camDir,glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,float& t,float& u,float& v);
	bool ifCollision(glm::vec3 nxtPos);

	~SceneManager(){};

private:
	unsigned int loadTexture(char const * path);
	unsigned int loadCubemap(vector<std::string> faces);
	
	vector<std::string> Faces
    {
        "../resource/skybox/right.jpg",
        "../resource/skybox/left.jpg",
        "../resource/skybox/top.jpg",
        "../resource/skybox/bottom.jpg",
        "../resource/skybox/front.jpg",
        "../resource/skybox/back.jpg"
    };
};

#endif