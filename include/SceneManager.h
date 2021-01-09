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
#include <windows.h>
#include <comdef.h>
#include <gdiplus/gdiplus.h>
#include <string>
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


namespace scene
{
	class SceneManager;
};

class SceneNode
{
public:
	glm::vec3 pos;
	glm::vec3 rotAxis;
	glm::vec3 scale;
	float rotAngle;

	TYPE type;

	bool active;
	// Texture

	int FaceName;

	SceneNode(TYPE Type);
	SceneNode(glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, TYPE Type,int id = 0);

	void draw(Shader &shader);

	~SceneNode(){
		//delete this;
	};
};

class SceneManager
{
public:
	GLFWwindow *window;
	Camera* camera;
	Shader* commonShader;
	Shader* lightShader;
	Shader* meshShader;
	bool collideMode;
	bool constructNode;
	bool destructNode;
	std::vector<Model> meshNodes;
	std::vector<Light> lights;
	std::vector<SceneNode> sceneNodes;

	inline SceneManager(GLFWwindow* mywindow){
		window = mywindow;
		commonShader = new Shader(ordinary_type);
		meshShader = new Shader(mesh_type);
		lightShader = new Shader(light_type);
		camera = new Camera(glm::vec3(0.0f,0.0f,3.0f));
	};

	//Each node uses a set of VBO, VAO and EBO(if need)
	unsigned int VBO[7];
	unsigned int VAO[7];
	unsigned int EBO[7];

	void setupSMGR();			//Used before the render loop to bind VBO, VAO and EBOs(if need)
	void deleteSMGR();			//Used at the end to delete those buffers

	void addMeshSceneNode(SceneManager *smgr, const char* path ,int id);

	void addCubeNode(SceneManager *smgr, int id);
	void addCubeNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, int id);

	void addSphereNode(SceneManager *smgr, int id);
	void addSphereNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, int id);

	void addCylinderNode(SceneManager *smgr, int id);
	void addCylinderNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, int id);

	void addConeNode(SceneManager *smgr, int id);
	void addConeNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, int id);

	void addPolyhedronNode(SceneManager *smgr, int id, int faceNum = 3);

	void addFrustumNode(SceneManager *smgr, int id, int faceNum = 3);

	Light *addLightNode(SceneManager *smgr, int id);

	Light *addFocusLightNode(SceneManager *smgr, int id);

	/* draw all the meshNodes and Nodes */
	void drawAll();

	/* auto save to current dir */
	void prtScreen();

	~SceneManager();

private:
	std::vector<SceneNode> commonNodes;

	/* The following arrays and functions generate those standard polygons' coordinates we need */

	std::vector<float> StdCube_Vertex;
	std::vector<int> StdCube_Index;
	void GenStdCube();

	std::vector<float> StdSphere_Vertex;
	std::vector<int> StdSphere_Index;
	void GenStdSphere();

	std::vector<float> StdCylinder_Vertex;
	std::vector<int> StdCylinder_Index;
	void GenStdCylinder();

	std::vector<float> StdCone_Vertex;
	std::vector<int> StdCone_Index;
	void GenStdCone();

	/* The following functions help print screen */
	bool CaptureScreenShot(	int nWidth, int nHeight, const std::wstring& szDestFile, const std::wstring& szEncoderString);
	int GetEncoderClsid(const WCHAR *format, CLSID *pClsid);
};

#endif