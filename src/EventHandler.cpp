#include "../include/EventHandler.h"
#include "../include/SceneManager.h"
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
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {      
        eventer->smgr->camera->ChangeGOD();
    }
    if(glfwGetKey(window,GLFW_KEY_Q) == GLFW_PRESS){
        /* change the cube */
    }
    if(glfwGetKey(window,GLFW_KEY_L) == GLFW_PRESS){
        /* move light */
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {   
        float temp = eventer->deltaTime;
        glm::vec3 nxtPos = eventer->smgr->camera->ProcessKeyboard(FORWARD,temp);
        if(eventer->smgr->ifCollision(nxtPos)){
            eventer->smgr->camera->ProcessKeyboard(BACKWARD,temp);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        float temp = eventer->deltaTime;
        glm::vec3 nxtPos = eventer->smgr->camera->ProcessKeyboard(BACKWARD,temp);
        if(eventer->smgr->ifCollision(nxtPos)){
            eventer->smgr->camera->ProcessKeyboard(FORWARD,temp);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        float temp = eventer->deltaTime;
        glm::vec3 nxtPos = eventer->smgr->camera->ProcessKeyboard(LEFT,temp);
        if(eventer->smgr->ifCollision(nxtPos)){
            eventer->smgr->camera->ProcessKeyboard(RIGHT,temp);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        float temp = eventer->deltaTime;
        glm::vec3 nxtPos = eventer->smgr->camera->ProcessKeyboard(RIGHT, temp);
        if(eventer->smgr->ifCollision(nxtPos)){
            eventer->smgr->camera->ProcessKeyboard(LEFT,temp);
        }
    }
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
		eventer->smgr->prtScreen();
	}
    if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS){
        float temp = eventer->deltaTime;
        glm::vec3 nxtPos = eventer->smgr->camera->ProcessKeyboard(UP,temp);
        if(eventer->smgr->ifCollision(nxtPos)){
            eventer->smgr->camera->ProcessKeyboard(DOWN,temp);
        }
    }
    if(glfwGetKey(window,GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        float temp = eventer->deltaTime;
        glm::vec3 nxtPos = eventer->smgr->camera->ProcessKeyboard(DOWN,temp);
        if(eventer->smgr->ifCollision(nxtPos)){
            eventer->smgr->camera->ProcessKeyboard(UP,temp);
        }
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