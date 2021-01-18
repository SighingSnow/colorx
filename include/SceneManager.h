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
#include <cstring>
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stepWidth*sizeof(float), (void*)0);
        
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stepWidth*sizeof(float), (void*)(3*sizeof(float)));
		
		glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stepWidth*sizeof(float), (void*)(6*sizeof(float)));
		
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

class Cross
{
public:
	inline Cross(){
		setUpCross();
	}
	
	void draw();
	
	~Cross(){};
private:
	unsigned int VAO,VBO;
	float crossVertices[60] = {
		-1.0,   0.0,	1.0, 0.0, 1.0,
		 0.0, -0.08,	0.0, 1.0, 1.0,
		 0.0,  0.08,	1.0, 1.0, 0.0,
		
		 1.0,   0.0,	1.0, 1.0, 0.0,
		 0.0,  0.08,	1.0, 1.0, 0.0,
		 0.0, -0.08,	0.0, 1.0, 1.0,
		
		  0.0, -1.0,	0.0, 1.0, 1.0,
		 0.08,  0.0,	1.0, 1.0, 0.0,
		-0.08,  0.0,	1.0, 0.0, 1.0,
		
		  0.0,  1.0,	1.0, 0.0, 0.0,
		-0.08,  0.0,	1.0, 0.0, 1.0,
		 0.08,  0.0,	1.0, 1.0, 0.0};
	void setUpCross()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(crossVertices), &crossVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2*sizeof(float)));
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
	Shader* crossShader;
	Cross ourCross;
	Skybox ourSkybox;
	Light* nodeLight;
	std::vector<Model> meshNodes;
	std::vector<SceneNode> commonNodes;

	bool wire = false;

	int wWidth, wHeight;

	unsigned int StoneTex,
				 ObsidianTex,
				 WoodTex,
				 BrickTex;

	unsigned int skyboxTex;

	inline SceneManager(GLFWwindow* mywindow){
		window = mywindow;
		commonShader = new Shader(ordinary_type);
		meshShader = new Shader(mesh_type);
		skyboxShader = new Shader(skybox_type);
		crossShader = new Shader(cross_type);
		camera = new Camera(glm::vec3(0.0f,0.0f,3.0f));
		StoneTex = loadTexture(StonePath);
		ObsidianTex = loadTexture(ObsidianPath);
		WoodTex = loadTexture(WoodPath);
		BrickTex = loadTexture(BrickPath);
		skyboxTex = loadCubemap(Faces);
		ourSkybox = *new Skybox(skyboxTex);
		ourCross = *new Cross();
		nodeLight = new Light();
		glfwGetWindowSize(window, &wWidth, &wHeight);
	};

	void addMeshSceneNode(const char* path );

	void addCubeNode(bool Texture = false, unsigned int TextureID = 0);
	void addCubeNode(transAttr transform, bool Texture = false, unsigned int TextureID = 0);

	void addSphereNode(bool Texture = false, unsigned int TextureID = 0);
	void addSphereNode(transAttr transform, bool Texture = false, unsigned int TextureID = 0);

	void addCylinderNode(bool Texture = false, unsigned int TextureID = 0);
	void addCylinderNode(transAttr transform, bool Texture = false, unsigned int TextureID = 0);

	void addConeNode(bool Texture = false, unsigned int TextureID = 0);
	void addConeNode(transAttr transform, bool Texture = false, unsigned int TextureID = 0);

	void addFrustumNode(bool Texture = false, unsigned int TextureID = 0);
	void addFrustumNode(transAttr transform, bool Texture = false, unsigned int TextureID = 0);

	void addPyramidNode(bool Texture = false, unsigned int TextureID = 0);
	void addPyramidNode(transAttr transform, bool Texture = false, unsigned int TextureID = 0);

	/* draw all the meshNodes and Nodes */
	void drawAll();

	/* auto save to current dir */
	void prtScreen();

	bool intersect(glm::vec3 camPos,glm::vec3 camDir,glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,float& t,float& u,float& v);
	bool ifCollision(glm::vec3 nxtPos);

	~SceneManager(){};

private:
	unsigned int loadTexture(std::string path);
	unsigned int loadCubemap(vector<std::string> faces);

#ifndef __APPLE__
	std::string StonePath = "../resource/texture_image/stone.png",
				ObsidianPath = "../resource/texture_image/obsidian.png",
				WoodPath = "../resource/texture_image/wood.png",
				BrickPath = "../resource/texture_image/brick.png";
	
	vector<std::string> Faces
    {
        "../resource/skybox/right.jpg",
        "../resource/skybox/left.jpg",
		"../resource/skybox/bottom.jpg",
        "../resource/skybox/top.jpg",
        "../resource/skybox/front.jpg",
        "../resource/skybox/back.jpg"
    };
#else
	std::string StonePath = "resource/texture_image/stone.png",
				ObsidianPath = "resource/texture_image/obsidian.png",
				WoodPath = "resource/texture_image/wood.png",
				BrickPath = "resource/texture_image/brick.png";
	
	vector<std::string> Faces
    {
        "resource/skybox/right.jpg",
        "resource/skybox/left.jpg",
		"resource/skybox/bottom.jpg",
        "resource/skybox/top.jpg",
        "resource/skybox/front.jpg",
        "resource/skybox/back.jpg"
    };
#endif

};

#endif