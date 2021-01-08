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
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include "EventHandler.h"
#include "Shader.h"
#include "SceneManager.h"
class Light : public EventHandler
{ 
public:
    glm::vec3 position;

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

    inline Light():position(glm::vec3(0.0,0.0,0.0)),
    ambient(glm::vec3(1.0,1.0,1.0)),
    diffuse(glm::vec3(1.0,1.0,1.0)),
    specular(glm::vec3(1.0,1.0,1.0)),
    index(0),
    isSelected(0)
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
    /* adjust the shiness and pos */
    /* use mouse_button_callback and keyboard_call_back */
    void OnEvent();

    /* GLFW provides glfwGetMousePos and many useful API for you to use */
    /* tutorial may is here https://www.glfw.org/docs/latest/input_guide.html#input_mouse_button */
    void mouse_button_callback(GLFWwindow* window,int button, int action, int mods);

    void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    Shader* lightShader;
    SceneManager* smgr;
    GLFWwindow* window;
};

#endif