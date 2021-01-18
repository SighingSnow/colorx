#ifndef CORE_H
#define CORE_H

#include <iostream>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

/* this should be use in glVertexAttrib */
const unsigned int stepWidth = 8;

const float Minier = 0.1;

enum pick_type
{
    unPickable,
    isPickable
};

enum shader_type
{
    ordinary_type,
    mesh_type,
	skybox_type,
	cross_type,
    light_type
};

const float PI = 3.14159265358979323846f;

static const char *nodeVShader = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
	"layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 FragPos;\n"
	"out vec3 Normal;\n"
	"out vec2 TexCoord;\n"

    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
        "FragPos = vec3(model * vec4(aPos,1.0));\n"
        "Normal = mat3(transpose(inverse(model))) * aNormal;\n"
		"TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
        "gl_Position = projection*view*model*vec4(aPos,1.0);\n"
    "}\0";
static const char *nodeFShader = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 Normal;  \n"
    "in vec3 FragPos;  \n"
	"in vec2 TexCoord;\n"

	"uniform bool EnableTexture;\n"
	"uniform sampler2D Texture;\n"

    "uniform vec3 lightPos; \n"
    "uniform vec3 viewPos; \n"
    "uniform vec3 lightColor;\n"
    "uniform vec3 objectColor;\n"
    "void main()\n"
    "{\n"
        // ambient
        "float ambientStrength = 0.5;\n"
        "vec3 ambient = ambientStrength * lightColor;\n"
        
        // diffuse 
        "vec3 norm = normalize(Normal);\n"
        "vec3 lightDir = normalize(lightPos - FragPos);\n"
        "float diff = max(dot(norm, lightDir), 0.0);\n"
        "vec3 diffuse = diff * lightColor;\n"
        
        // specular
        "float specularStrength = 0.3;\n"
        "vec3 viewDir = normalize(viewPos - FragPos);\n"
        "vec3 reflectDir = reflect(-lightDir, norm);  \n"
        "float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
        "vec3 specular = specularStrength * spec * lightColor;  \n"
        
        "vec3 material = (ambient + diffuse + specular) * objectColor;\n"
		"vec4 TexResult = EnableTexture ? texture(Texture, TexCoord) : vec4(1.0);\n"
        "FragColor = vec4(material, 1.0) * TexResult;\n"
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
    "uniform bool hasTexture;\n"
    "uniform vec3 diffuse;\n"
    "void main()\n"
    "{\n"
    "   FragColor = hasTexture ? texture(texture_diffuse1,TexCoords) : vec4(diffuse,1.0);\n"
    "}\n\0";

static const char *skyboxVShader = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"out vec3 TexCoords;\n"
	"uniform mat4 projection;\n"
	"uniform mat4 view;\n"
	"void main()\n"
	"{\n"
	"	TexCoords = vec3(aPos.x, -aPos.y, aPos.z);\n"
	"	vec4 pos = projection * view * vec4(aPos, 1.0);\n"
	"	gl_Position = pos.xyww;\n"
	"}\n\0";
static const char *skyboxFShader = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 TexCoords;\n"
	"uniform samplerCube skybox;\n"
	"void main()\n"
	"{\n"
	"	FragColor = texture(skybox, TexCoords);\n"
	"}\n\0";

static const char *crossVShader = "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"out vec3 Color;\n"
	"uniform vec2 scrCoeff;\n"
    "void main()\n"
    "{\n"
	"	Color = aColor;\n"
	"	vec2 pos = scrCoeff * aPos;\n"
    "   gl_Position = vec4(pos, 0.0, 1.0);\n"
    "}\0";
static const char *crossFShader = "#version 330 core\n"
	"in vec3 Color;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(Color, 0.2);\n"
    "}\n\0";

#endif