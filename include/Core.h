#ifndef CORE_H
#define CORE_H

#include <iostream>

const float SCR_WIDTH = 600;
const float SCR_HEIGHT = 800;

enum picktype
{
    unPickable,
    isPickable
};

const float PI = 3.14159265358979323846f;

static const char *nodeVShader = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
static const char *nodeFShader = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

#endif