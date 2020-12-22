/*
 * View:
 * This is main frame of the game
 */

#ifndef COLORX_H
#define COLORX_H

#include <glfw/glfw3.h>
#include "SceneManager.h"

class Colorx
{ 
public:
    GLFWwindow* window;
    SceneManager* smgr;

    inline Colorx(){
        window = nullptr;
        smgr = nullptr;
    }

    void init();

    /* jugdge if GLFWwindow active */
    bool run();
};

#endif