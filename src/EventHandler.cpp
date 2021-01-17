#include "../include/EventHandler.h"
bool firstMouse = false;
bool focused = true;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

void EventHandler::getTransMat(const SceneNode snode,glm::mat4 & trans)
{
    glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, snode.NodeAttr.Scale);
    model = glm::rotate(model,snode.NodeAttr.RotAngle, snode.NodeAttr.RotAxis);
	model = glm::translate(model, snode.NodeAttr.Position);
    trans = model;
    return;
}


void processInput(GLFWwindow *window)
{
    void* data = glfwGetWindowUserPointer(window);
    EventHandler *eventer = static_cast<EventHandler*>(data);
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {      
        eventer->smgr->camera->ChangeGOD();
    }
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {      
        focused = !focused;
		if(focused)		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
		eventer->smgr->prtScreen();
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
		eventer->smgr->wire = !eventer->smgr->wire;
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

void mouse_button_callback(GLFWwindow* window,int button, int action, int mods)
{  
    void* data = glfwGetWindowUserPointer(window);
    EventHandler *eventer = static_cast<EventHandler*>(data);
    glm::mat4 mat;
    /* when collide cube, construct a new one */
    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        /* set smgr render mode */
        glm::mat4 trans;
        transAttr tAttr;
		bool tTex;
		unsigned int tTexID;
        float t,u,v,tmpT;
        glm::vec3 e1,e2;
        glm::vec3 faceNorm;
        t = tmpT = FLT_MAX; /* the maxium of float */
        int target  = -1;
        std::vector<SceneNode> tmpCNodes = eventer->smgr->commonNodes;
        for(int i = 0;i < tmpCNodes.size();i++){
            eventer->getTransMat(tmpCNodes[i],trans);
            if(tmpCNodes[i].type == _CUBE_ && tmpCNodes[i].NodeAttr.pick == isPickable && tmpCNodes[i].NodeAttr.isAlive){
                eventer->getTransMat(tmpCNodes[i],trans);
                for(int j = 0;j < tmpCNodes[i].indices.size();j+=3){
                    int tmpIndices = tmpCNodes[i].indices[j];
                    
                    glm::vec3 v1(tmpCNodes[i].vertices[tmpIndices*stepWidth],tmpCNodes[i].vertices[tmpIndices*stepWidth+1],tmpCNodes[i].vertices[tmpIndices*stepWidth+2]);
                    glm::vec3 v2(tmpCNodes[i].vertices[(tmpIndices+1)*stepWidth],tmpCNodes[i].vertices[(tmpIndices+1)*stepWidth+1],tmpCNodes[i].vertices[(tmpIndices+1)*stepWidth+2]);
                    glm::vec3 v3(tmpCNodes[i].vertices[(tmpIndices+2)*stepWidth],tmpCNodes[i].vertices[(tmpIndices+2)*stepWidth+1],tmpCNodes[i].vertices[(tmpIndices+2)*stepWidth+2]);
                    
                    glm::vec4 pos1(v1,1.0); glm::vec4 pos2(v2,1.0); glm::vec4 pos3(v3,1.0);
                    glm::vec4 tmp1 = trans*pos1; glm::vec4 tmp2 = trans*pos2; glm::vec4 tmp3 = trans*pos3;
                    glm::vec3 p1,p2,p3;
                    memcpy(&p1,&tmp1,sizeof(float)*3);
                    memcpy(&p2,&tmp2,sizeof(float)*3);
                    memcpy(&p3,&tmp3,sizeof(float)*3);

                    if(eventer->smgr->intersect(eventer->smgr->camera->getPostion(),eventer->smgr->camera->getDirection(),p1,p2,p3,t,u,v)){
                        if(t < tmpT && t > 0) {
                            tmpT = t;
                            //std::cout<<"[T]"<<t<<std::endl;
                            target = i;
                            e1 = v2-v1;
                            e2 = v3 -v1;
                            // if(glm::cross(e1,e2) == glm::vec3(0,0,1)) printf("HIT FACE\n");
                            // else if(glm::cross(e1,e2) == glm::vec3(0,-1,0)) printf("HIT BUTTOM\n");
                            // else if(glm::cross(e1,e2) == glm::vec3(0,1,0)) printf("HIT UP\n");
                        }
                    }

                }
            }
        }
        
        faceNorm = cross(e1,e2);
        
        faceNorm = glm::normalize(faceNorm);
        glm::vec3 camDir = eventer->smgr->camera->getDirection();
        glm::vec3 camPos = eventer->smgr->camera->getPostion();
        if(dot(faceNorm,camDir) > 0) faceNorm = -faceNorm;

        // std::cout<<"[Target]"<<target<<std::endl;
        // std::cout<<"[POS]"<<camPos[0]<<camPos[1]<<camPos[2]<<std::endl;;
        
        tAttr = eventer->smgr->commonNodes[target].NodeAttr;
		tTex = eventer->smgr->commonNodes[target].texture;
		tTexID = eventer->smgr->commonNodes[target].textureID;
        if(target != -1){
            //std::cout<<"[ORIGIN]"<<tAttr.Position[0]<<":"<<tAttr.Position[1]<<":"<<tAttr.Position[2]<<std::endl;
            tAttr.Position +=  faceNorm;
            //std::cout<<"[RESULT]"<<tAttr.Position[0]<<":"<<tAttr.Position[1]<<":"<<tAttr.Position[2]<<std::endl;
            eventer->smgr->addCubeNode(tAttr,tTex,tTexID);
        }
        
    }
    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        glm::vec3 testVec;
        glm::mat4 trans;
        float t,u,v,tmpT;
        t = tmpT = FLT_MAX; /* the maxium of float */
        int target  = -1;
        std::vector<SceneNode> tmpCNodes = eventer->smgr->commonNodes;
        for(int i = 0;i < eventer->smgr->commonNodes.size();i++){
            if(tmpCNodes[i].NodeAttr.pick == isPickable && tmpCNodes[i].NodeAttr.isAlive){
                eventer->getTransMat(tmpCNodes[i],trans);
                for(int j = 0;j < eventer->smgr->commonNodes[i].indices.size();j+=3){
                    int tmpIndices = tmpCNodes[i].indices[j];
                    
                    glm::vec3 p1(tmpCNodes[i].vertices[tmpIndices*stepWidth],tmpCNodes[i].vertices[tmpIndices*stepWidth+1],tmpCNodes[i].vertices[tmpIndices*stepWidth+2]);
                    glm::vec3 p2(tmpCNodes[i].vertices[(tmpIndices+1)*stepWidth],tmpCNodes[i].vertices[(tmpIndices+1)*stepWidth+1],tmpCNodes[i].vertices[(tmpIndices+1)*stepWidth+2]);
                    glm::vec3 p3(tmpCNodes[i].vertices[(tmpIndices+2)*stepWidth],tmpCNodes[i].vertices[(tmpIndices+2)*stepWidth+1],tmpCNodes[i].vertices[(tmpIndices+2)*stepWidth+2]);
                    
                    glm::vec4 pos1(p1,1.0); glm::vec4 pos2(p2,1.0); glm::vec4 pos3(p3,1.0);
                    glm::vec4 tmp1 = trans*pos1; glm::vec4 tmp2 = trans*pos2; glm::vec4 tmp3 = trans*pos3;
                    memcpy(&p1,&tmp1,sizeof(float)*3);
                    memcpy(&p2,&tmp2,sizeof(float)*3);
                    memcpy(&p3,&tmp3,sizeof(float)*3);

                    if(eventer->smgr->intersect(eventer->smgr->camera->getPostion(),eventer->smgr->camera->getDirection(),p1,p2,p3,t,u,v)){
                        if(t < tmpT && t > 0) {
                            tmpT = t;
                            target = i;
                        }
                    }

                }
            }
        }
        if(target >= 0) eventer->smgr->commonNodes[target].NodeAttr.isAlive = false;
    }
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

    if(focused)
		eventer->smgr->camera->ProcessMouseMovement(xoffset, yoffset);

    return;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}