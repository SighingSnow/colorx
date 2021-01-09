/*
 * View:
 * This is main frame of the game
 */
#pragma once
#ifndef COLORX_H
#define COLORX_H

#include "EventHandler.h"

class Colorx
{ 
public:
    GLFWwindow* window;
    SceneManager* smgr;
    EventHandler* eventer;
    Colorx();

    void initScene();

    /* judge if GLFWwindow active */
    void run();

    ~Colorx();
};

#endif