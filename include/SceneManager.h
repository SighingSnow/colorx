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
};

typedef struct _TRANSFORM_ATTRIBUTE_ transAttr;

class SceneNode
{
public:
	TYPE type;

	transAttr NodeAttr;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	bool active;
	// Texture

	inline SceneNode(TYPE Type)
	{
		type = Type;
		NodeAttr.Position	= glm::vec3(0.0);
		NodeAttr.RotAngle	= 0;
		NodeAttr.RotAxis	= glm::vec3(1.0, 0.0, 0.0);
		NodeAttr.Scale		= glm::vec3(1.0);
		NodeAttr.Color		= glm::vec3(1.0);
		NodeAttr.FaceNum	= 3;
		NodeAttr.Ratio		= 1;
		setUpSceneNode(type);
	}

	inline SceneNode(transAttr transform, TYPE Type, int id = 0 )
	{
		type				= Type;
		NodeAttr.Position	= transform.Position;
		NodeAttr.RotAngle	= glm::radians(transform.RotAngle);
		NodeAttr.RotAxis	= transform.RotAxis;
		NodeAttr.Scale		= transform.Scale;
		NodeAttr.Color		= transform.Color;
		NodeAttr.FaceNum	= Type == _CYLINDER_ || Type == _CONE_ ? 100 : transform.FaceNum;
		
		if		(Type == _FRUSTUM_ 	|| Type == _PYRAMID_)		NodeAttr.FaceNum = transform.FaceNum;
		else if (Type == _CONE_		|| Type == _CYLINDER_)		NodeAttr.FaceNum = 100;
		
		if		(Type == _FRUSTUM_ 	|| Type == _CYLINDER_)		NodeAttr.Ratio = transform.Ratio;
		else if (Type == _CONE_		|| Type == _PYRAMID_)		NodeAttr.Ratio = 0;
		
		setUpSceneNode(type);
	}

	void draw(Shader &shader);

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
		
		// glEnableVertexAttribArray(2);
        // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
		
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
	bool collideMode;
	bool constructNode;
	bool destructNode;
	std::vector<Model> meshNodes;
	std::vector<Light> lights;
	std::vector<SceneNode> commonNodes;

	bool wire = false;

	inline SceneManager(GLFWwindow* mywindow){
		window = mywindow;
		commonShader = new Shader(ordinary_type);
		meshShader = new Shader(mesh_type);
		camera = new Camera(glm::vec3(0.0f,0.0f,3.0f));
	};

	void addMeshSceneNode(SceneManager *smgr, const char* path ,int id);

	void addCubeNode(SceneManager *smgr, int id);
	void addCubeNode(SceneManager *smgr, transAttr transform, int id);

	void addSphereNode(SceneManager *smgr, int id);
	void addSphereNode(SceneManager *smgr, transAttr transform, int id);

	void addCylinderNode(SceneManager *smgr, int id);
	void addCylinderNode(SceneManager *smgr, transAttr transform, int id);

	void addConeNode(SceneManager *smgr, int id);
	void addConeNode(SceneManager *smgr, transAttr transform, int id);

	void addFrustumNode(SceneManager *smgr, int id);
	void addFrustumNode(SceneManager *smgr, transAttr transform, int id);

	void addPyramidNode(SceneManager *smgr, int id);
	void addPyramidNode(SceneManager *smgr, transAttr transform, int id);

	/* draw all the meshNodes and Nodes */
	void drawAll();

	/* auto save to current dir */
	void prtScreen();

	~SceneManager();

private:
	
};

#endif