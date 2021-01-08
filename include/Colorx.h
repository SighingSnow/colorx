/*
 * View:
 * This is main frame of the game
 */

#ifndef COLORX_H
#define COLORX_H

#include <glfw/glfw3.h>
#include "SceneManager.h"
#include "EventHandler.h"
#include "Core.h"
#include "Camera.h"

extern bool firstMouse;
extern float lastX;
extern float lastY;

class Colorx
{ 
public:
    GLFWwindow* window;
    SceneManager* smgr;
    EventHandler* eventer;
    Shader* shader;

    Colorx();

    void initScene();

    /* judge if GLFWwindow active */
    void run();

    ~Colorx();
};

#endif