/*
 * Function:
 * Camera / Light / Mesh is all in here
 */

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <iostream>
#include <vector>
#include <glfw/glfw3.h>
#include "Light.h"
#include "Camera.h"
#include "Model.h"

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

typedef enum _TYPE_ TYPE

#include "SceneManager.cpp"

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
	SceneNode(glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, TYPE Type);

	void draw(Shader &shader);

	~SceneNode();
};

class SceneManager
{
public:
	std::vector<Model> meshNodes;
	std::vector<Light> lights;

	//Each node uses a set of VBO, VAO and EBO(if need)
	unsigned int VBO[7];
	unsigned int VAO[7];
	unsigned int EBO[7];

	void setupSMGR();			//Used before the render loop to bind VBO, VAO and EBOs(if need)
	void deleteSMGR();			//Used at the end to delete those buffers

	void addMeshSceneNode(SceneManager *smgr, int id = 0);

	void addCubeNode(SceneManager *smgr, int id = 0);
	void addCubeNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, int id = 0);

	void addSphereNode(SceneManager *smgr, int id = 0);
	void addSphereNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, int id = 0);

	void addCylinderNode(SceneManager *smgr, int id = 0);
	void addCylinderNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, int id = 0);

	void addConeNode(SceneManager *smgr, int id = 0);
	void addConeNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, int id = 0);

	void addPolyhedronNode(SceneManager *smgr, int id = 0, int faceNum = 3);

	void addFrustumNode(SceneManager *smgr, int id = 0, int faceNum = 3);

	Light *addLightNode(SceneManager *smgr, int id = 0);

	Light *addFocusLightNode(SceneManager *smgr, int id = 0);

	Camera *addFpsCameraNode(SceneManager *smgr);
	Camera *addFpsCameraNode(SceneManager *smgr, glm::vec3 Pos, glm::vec3 Up);

	/* draw all the meshNodes and Nodes */
	void drawAll(Shader &shader);

	/* auto save to current dir */
	void prtScreen();

	SceneManager();
	~SceneManager();

private:
	std::vector<SceneNode> commonNodes;

	GLFWwindow *window;

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
};

#endif