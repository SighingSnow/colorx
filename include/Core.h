#ifndef CORE_H
#define CORE_H

#include <iostream>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

enum pick_type
{
    unPickable,
    isPickable
};

enum shader_type
{
    ordinary_type,
    mesh_type,
    light_type
};

const float PI = 3.14159265358979323846f;

static const char *nodeVShader = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoords;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"

    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
        "TexCoords = aTexCoords;\n"
        "ourColor = aColor;\n"
    "   gl_Position = projection*view*model*vec4(aPos,1.0);\n"
    "}\0";
static const char *nodeFShader = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoords;\n"
    "uniform sampler2D texture1;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(texture1,TexCoord);\n"
    "}\n\0";

static const char *meshNodeVShader = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoords;\n"
    "out vec2 TexCoords;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
        "TexCoords = aTexCoords;\n"
    "   gl_Position = projection*view*model*vec4(aPos,1.0);\n"
    "}\0";

static const char *meshNodeFShader = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoords;\n"
    "uniform sampler2D texture_diffuse1;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(texture_diffuse1,TexCoords);\n"
    "}\n\0";

static const char *lightVShader = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoords;\n"
    "out vec2 TexCoords;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
        "TexCoords = aTexCoords;\n"
    "   gl_Position = projection*view*model*vec4(aPos,1.0);\n"
    "}\0";
static const char *lightFShader = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoords;\n"
    "uniform sampler2D texture_diffuse1;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(texture_diffuse1,TexCoords);\n"
    "}\n\0";

#endif