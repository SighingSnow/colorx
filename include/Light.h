/*
 * 
 * Light:
 * addLight
 * addFocusLight
 * addToolLight
 * 
 */

#ifndef LIGHT_H
#define LIGHT_H
#include <glm/glm.hpp>
#include "Shader.h"
#include <GLFW/glfw3.h>

class Light
{ 
public:
    glm::vec3 position;
    Shader* lightShader;

    float constant;
    float linear;
    float quadratic;
    float strength = 1.0;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    /* You can add more variants here if you need */

    int index;
    /* judge whether the light can be modified */
    bool isSelected;

    inline Light():position(glm::vec3(1.2,1.0,3.0)),
    ambient(glm::vec3(1.0,1.0,1.0)),
    diffuse(glm::vec3(1.0,1.0,1.0)),
    specular(glm::vec3(1.0,1.0,1.0)),
    index(0),
    isSelected(0),
    strength(0.5)
    {

    }

    inline Light(
        glm::vec3 position,
        glm::vec3 direction,
        glm::vec3 Ambient = glm::vec3(1.0,1.0,1.0),
        glm::vec3 diffuse = glm::vec3(1.0,1.0,1.0),
        glm::vec3 specular = glm::vec3(1.0,1.0,1.0)
    ):isSelected(0),index(0){

    };
    
    void setLight(Shader* lightShader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void drawLight(Shader* lightShader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void drawDirectionLight(Shader* lightShader, glm::vec3 direction);

    void drawPointLight(Shader* lightShader, glm::vec3 direction);

private:
    GLFWwindow* window;
};

#endif