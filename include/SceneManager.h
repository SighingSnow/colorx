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
	_POLYHEDRON_,
	_FRUSTUM_
};

typedef enum _TYPE_ TYPE;

class SceneNode
{
public:
	glm::vec3 pos;
	glm::vec3 rotAxis;
	glm::vec3 scale;
	glm::vec3 color;
	float rotAngle;
	
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;
	TYPE type;

	bool active;
	// Texture

	int FaceName;

	inline SceneNode(TYPE Type)
	{
		type = Type;
		pos= glm::vec3(0.0, 0.0, 0.0);
		rotAngle = 0;
		color = glm::vec3(1.0,1.0,1.0);
		rotAxis	= glm::vec3(1.0, 0.0, 0.0);
		scale = glm::vec3(1.0, 1.0, 1.0);
		setUpSceneNode(type);
	}

	inline SceneNode(glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale,glm::vec3 Color ,TYPE Type, int id = 0 )
	{
		pos				= Pos;
		rotAngle		= glm::radians(RotAngle);
		rotAxis			= RotAxis;
		scale			= Scale;
		type			= Type;
		color 			= Color;
		setUpSceneNode(type);
	}

	void draw(Shader &shader);

	~SceneNode(){
		//delete this;
	};
private:
	unsigned int VBO,EBO;
	
	void GenStdCube();
	void GenStdSphere();

	void GenStdCylinder();
	void GenStdCone();

	void setUpSceneNode(TYPE type){
		switch(type){
			case _CUBE_: GenStdCube(); break;
			case _CONE_: GenStdCone(); break;
			case _SPHERE_: GenStdSphere(); break;
			case _CYLINDER_: GenStdCylinder(); break;
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

	inline SceneManager(GLFWwindow* mywindow){
		window = mywindow;
		commonShader = new Shader(ordinary_type);
		meshShader = new Shader(mesh_type);
		camera = new Camera(glm::vec3(0.0f,0.0f,3.0f));
	};

	void addMeshSceneNode(SceneManager *smgr, const char* path ,int id);

	void addCubeNode(SceneManager *smgr, int id);
	void addCubeNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale,glm::vec3 Color , int id);

	void addSphereNode(SceneManager *smgr, int id);
	void addSphereNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, glm::vec3 Color ,int id);

	void addCylinderNode(SceneManager *smgr, int id);
	void addCylinderNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale,glm::vec3 Color , int id);

	void addConeNode(SceneManager *smgr, int id);
	void addConeNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale,glm::vec3 Color , int id);

	void addPolyhedronNode(SceneManager *smgr, int id, int faceNum = 3);

	void addFrustumNode(SceneManager *smgr, int id, int faceNum = 3);

	/* draw all the meshNodes and Nodes */
	void drawAll();

	/* auto save to current dir */
	void prtScreen();

	~SceneManager();

private:
	

	/* The following arrays and functions generate those standard polygons' coordinates we need */
};

#endif