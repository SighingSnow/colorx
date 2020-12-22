#ifndef CONTEXT_MANAGER_H
#define CONTEXT_MANAGER_H

#include <iostream>

class ContextManager
{
    void beginScene();

    void endScene();

    /* init glfw */
    void initContext();

    /* load texture and bind*/
    void loadTexture(std::string path);
};

#endif