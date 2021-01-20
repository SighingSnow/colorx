#pragma once
#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <glm/glm.hpp>
#include "SceneManager.h"

extern bool firstMouse;
extern float lastX;
extern float lastY;

#define GravityAcceler -0.5f
#define JumpInitialSpeed 0.12f
#define JumpFactor 0.004f          //跳起速度系数
#define GravityFactor 0.004f       //下落速度系数

extern glm::vec3 velocity;        //垂直方向速度
extern glm::vec3 gravity;         //重力加速度
extern glm::vec3 accelerUp; 
extern int state; 

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void mouse_button_callback(GLFWwindow* window,int button, int action, int mods);

class EventHandler
{
public:
    SceneManager* smgr;

    float deltaTime;	// time between current frame and last frame
    float lastFrame;
    int targetObject;
    bool createMode;
    bool selectMode;

    inline EventHandler(SceneManager* sengr){
        smgr = sengr;
        deltaTime = 0.0f;
        lastFrame = 0.0f;
        glfwSetWindowUserPointer(smgr->window,this);
        glfwSetCursorPosCallback(smgr->window,mouse_callback);
        glfwSetScrollCallback(smgr->window,scroll_callback);
        glfwSetFramebufferSizeCallback(smgr->window,framebuffer_size_callback);
        glfwSetMouseButtonCallback(smgr->window,mouse_button_callback);
        createMode = false;
        selectMode = true;
    }

    void getTransMat(const SceneNode snode,glm::mat4 & trans);

};

#endif