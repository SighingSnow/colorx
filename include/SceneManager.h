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

namespace scene
{
    class SceneManager;
};

class SceneNode
{
public:
    glm::vec3 pos;
    glm::vec3 rotation;
    glm::vec3 scale;

    bool active;
    // Texture

    float length;
    float radius;
    float height;

    inline SceneNode():pos(glm::vec3(0.0,0.0,0.0)),rotation(glm::vec3(0.0,0.0,0.0)),scale(glm::vec3(1.0,1.0,1.0)),radius(1.0),height(2.0){}

    inline SceneNode(glm::vec3 Pos,glm::vec3 Rotation,glm::vec3 Scale,int R,int H){
        pos = Pos;
        rotation = Rotation;
        scale = Scale;
        radius = R;
        height = H;
    }
    
    void draw(const Shader & shader);
};

class SceneManager
{
public:
    std::vector<Model> meshNodes;
    std::vector<SceneNode> commonNodes;
    std::vector<Light> lights;

    SceneNode* addMeshSceneNode(SceneManager* smgr,int id = 0);

    SceneNode* addCubeNode(SceneManager* smgr,int id = 0);

    SceneNode* addSphereNode(SceneManager* smgr,int id = 0);

    SceneNode* addCylinderNode(SceneManager* smgr,int id = 0,int r = 1.0);

    SceneNode* addConeNode(SceneManager* smgr,int id = 0,int r = 1.0);

    SceneNode* addPolygonNode(SceneManager* smgr,int id = 0,int faceNum = 3);

    SceneNode* addFrustumNode(SceneManager* smgr,int id = 0,int faceNum = 3);

    Light* addLightNode(SceneManager* smgr,int id = 0);
    
    Light* addFocusLightNode(SceneManager* smgr,int id = 0);

    Camera* addFpsCameraNode(SceneManager* smgr);

    /* draw all the meshNodes and Nodes */
    void drawAll(); 

    /* auto save to current dir */
    void prtScreen();

private:
    GLFWwindow* window;
};

#endif