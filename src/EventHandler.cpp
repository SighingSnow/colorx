#include "../include/EventHandler.h"

bool firstMouse = false;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

void processInput(GLFWwindow *window)
{
     void* data = glfwGetWindowUserPointer(window);
    EventHandler *eventer = static_cast<EventHandler*>(data);
    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        /* set smgr render mode */
        eventer->smgr->collideMode = true;
        eventer->smgr->constructNode = true;
    }
    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        eventer->smgr->collideMode = true;
        eventer->smgr->destructNode = true;
    }
    if(glfwGetKey(window,GLFW_KEY_Q) == GLFW_PRESS){
        /* change the cube */
    }
    if(glfwGetKey(window,GLFW_KEY_L) == GLFW_PRESS){
        /* move light */
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {      
        eventer->smgr->camera->ProcessKeyboard(FORWARD,eventer->deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        eventer->smgr->camera->ProcessKeyboard(BACKWARD,eventer->deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        eventer->smgr->camera->ProcessKeyboard(LEFT,eventer->deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        eventer->smgr->camera->ProcessKeyboard(RIGHT, eventer->deltaTime);
    }

    return;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    void* data = glfwGetWindowUserPointer(window);
    EventHandler *eventer = static_cast<EventHandler*>(data);
    eventer->smgr->camera->ProcessMouseScroll(yoffset);
    return;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    void* data = glfwGetWindowUserPointer(window);
    EventHandler *eventer = static_cast<EventHandler*>(data);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    eventer->smgr->camera->ProcessMouseMovement(xoffset, yoffset);

    return;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}