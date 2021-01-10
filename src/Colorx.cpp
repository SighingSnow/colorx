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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glEnable(GL_DEPTH_TEST);

    return;
}

void Colorx::initScene()
{
<<<<<<< HEAD
    //smgr->addCubeNode(smgr,glm::vec3(0,0,0),0.0f,glm::vec3(0,0,0),glm::vec3(1,1,1),unPickable);
    smgr->addCubeNode(smgr,unPickable);
    // std::cout<<smgr->commonNodes.size()<<std::endl;
    // std::cout<<smgr->commonNodes[0].type<<std::endl;
=======
    smgr->setupSMGR();

	smgr->addCubeNode(smgr,glm::vec3(0,0,0),0.0f,glm::vec3(0,0,0),glm::vec3(1,1,1),unPickable);
>>>>>>> f6d7991eb94746bf7789691c27c929a228e31005
    //smgr->addMeshSceneNode(smgr,"resourse/nanosuit",unPickable);
    ///std::cout<<smgr->commonNodes[0].<<std::endl;
    // smgr->addConeNode(smgr,glm::vec3(1.0,0,0),0.0f,glm::vec3(0,0,0),glm::vec3(1,1,1),unPickable);
    // smgr->addConeNode(smgr,glm::vec3(-1.0,0,0),0.0f,glm::vec3(0,0,0),glm::vec3(1,1,1),unPickable);
}

void Colorx::run()
{   
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        eventer->deltaTime = currentFrame - eventer->lastFrame;
        eventer->lastFrame = currentFrame;
        processInput(window);
        
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        smgr->commonShader->use();
        smgr->meshShader->use();
        glm::mat4 projection = glm::perspective(glm::radians(smgr->camera->Zoom),(float)SCR_WIDTH/SCR_HEIGHT,0.1f,100.0f);
        glm::mat4 view = smgr->camera->GetViewMatrix();
        smgr->commonShader->setMat4("projection",projection);
        smgr->commonShader->setMat4("view",view);
        smgr->commonShader->setVec3("objectColor",1.0f,0.51f,0.13f);
        smgr->commonShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        smgr->commonShader->setMat4("model", model);

        smgr->meshShader->setMat4("projection", projection);
        smgr->meshShader->setMat4("view", view);
        smgr->meshShader->setMat4("model", model);

        smgr->drawAll();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return;
}

Colorx::~Colorx(){

}