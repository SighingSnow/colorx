#pragma once
#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <glm/glm.hpp>
#include "SceneManager.h"

extern bool firstMouse;
extern float lastX;
extern float lastY;


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

    inline EventHandler(SceneManager* sengr){
        smgr = sengr;
        deltaTime = 0.0f;
        lastFrame = 0.0f;
        glfwSetWindowUserPointer(smgr->window,this);
        glfwSetCursorPosCallback(smgr->window,mouse_callback);
        glfwSetScrollCallback(smgr->window,scroll_callback);
        glfwSetFramebufferSizeCallback(smgr->window,framebuffer_size_callback);
        glfwSetMouseButtonCallback(smgr->window,mouse_button_callback);
    }

    void getTransMat(const SceneNode snode,glm::mat4 & trans);
};

#endif