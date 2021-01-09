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

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(0);
    }
    glfwMakeContextCurrent(window);
    
    Shader shader((shaderPath+"nodeShader.vs").c_str(),(shaderPath+"nodeShader.fs").c_str());
    /* pass the window and auto set light and camera */
    smgr = new SceneManager(window);
    /* auto set callback functions */
    eventer = new EventHandler(smgr);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* configure glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(0);
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    //stbi_set_flip_vertically_on_load(true);

    return;
}

void Colorx::initScene()
{
    smgr->addConeNode(smgr,glm::vec3(1.0,0,0),0.0f,glm::vec3(0,0,0),glm::vec3(1,1,1),unPickable);
    smgr->addConeNode(smgr,glm::vec3(-1.0,0,0),0.0f,glm::vec3(0,0,0),glm::vec3(1,1,1),unPickable);

}

void Colorx::run()
{   
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        eventer->deltaTime = currentFrame - eventer->lastFrame;
        processInput(window);
        smgr->drawAll(*shader);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

Colorx::~Colorx(){

}