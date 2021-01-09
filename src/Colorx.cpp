#include "../include/Colorx.h"

Colorx::Colorx()
{
    glfwInit();
    if(!glfwInit())
    {
        std::cout<<"[WARNING]Failed to initialize GLFW"<<std::endl;
        exit(0);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Colorx", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(0);
    }
    glfwMakeContextCurrent(window);
    
     /* configure glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(0);
    }

    /* pass the window and auto set light and camera */
    smgr = new SceneManager(window);
    /* auto set callback functions */
    eventer = new EventHandler(smgr);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   

    return;
}

void Colorx::initScene()
{
    smgr->addCubeNode(smgr,glm::vec3(1.0,0,0),0.0f,glm::vec3(0,0,0),glm::vec3(1,1,1),unPickable);
    // smgr->addConeNode(smgr,glm::vec3(1.0,0,0),0.0f,glm::vec3(0,0,0),glm::vec3(1,1,1),unPickable);
    // smgr->addConeNode(smgr,glm::vec3(-1.0,0,0),0.0f,glm::vec3(0,0,0),glm::vec3(1,1,1),unPickable);
}

void Colorx::run()
{   
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        eventer->deltaTime = currentFrame - eventer->lastFrame;
        processInput(window);
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        smgr->commonShader->use();
        glm::mat4 projection = glm::perspective(glm::radians(smgr->camera->Zoom),SCR_WIDTH/SCR_HEIGHT,0.1f,100.0f);
        glm::mat4 view = smgr->camera->GetViewMatrix();
        smgr->commonShader->setMat4("projection",projection);
        smgr->commonShader->setMat4("view",view);

        smgr->drawAll(*(smgr->commonShader));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

Colorx::~Colorx(){

}