#ifndef CORE_H
#define CORE_H

const int SCR_WIDTH = 600;
const int SCR_HEIGHT = 800;

bool firstMouse = false;

float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;

enum picktype
{
    unPickable,
    isPickable
};

#endif