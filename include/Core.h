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
    "layout (location = 1) in vec3 aNormal;\n"
	// "layout (location = 2) in vec2 aTexCoords;\n"
    "out vec3 FragPos;\n"
	"out vec3 Normal;\n"

    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
        "FragPos = vec3(model * vec4(aPos,1.0));\n"
        "Normal = mat3(transpose(inverse(model))) * aNormal;\n"
        "gl_Position = projection*view*model*vec4(aPos,1.0);\n"
    "}\0";
static const char *nodeFShader = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 Normal;  \n"
    "in vec3 FragPos;  \n"

    "uniform vec3 lightPos; \n"
    "uniform vec3 viewPos; \n"
    "uniform vec3 lightColor;\n"
    "uniform vec3 objectColor;\n"
    "void main()\n"
    "{\n"
        // ambient
        "float ambientStrength = 0.1;\n"
        "vec3 ambient = ambientStrength * lightColor;\n"
        
        // diffuse 
        "vec3 norm = normalize(Normal);\n"
        "vec3 lightDir = normalize(lightPos - FragPos);\n"
        "float diff = max(dot(norm, lightDir), 0.0);\n"
        "vec3 diffuse = diff * lightColor;\n"
        
        // specular
        "float specularStrength = 0.5;\n"
        "vec3 viewDir = normalize(viewPos - FragPos);\n"
        "vec3 reflectDir = reflect(-lightDir, norm);  \n"
        "float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
        "vec3 specular = specularStrength * spec * lightColor;  \n"
        
        "vec3 result = (ambient + diffuse + specular) * objectColor;\n"
        "FragColor = vec4(result, 1.0);\n"
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

#endif