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
	glfwWindowHint(GLFW_SAMPLES, 4);

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

    glEnable(GL_DEPTH_TEST);

    return;
}

void Colorx::initScene()
{
	transAttr transform;
	transform.Position = glm::vec3(-7,0,0);
	transform.RotAngle = -90.0f;
	transform.RotAxis = glm::vec3(1,0,0);
	transform.Scale = glm::vec3(Minier);
	transform.Color = glm::vec3(1.0,0.5,0.31);
	transform.FaceNum = 3;
	transform.Ratio = 1;
    transform.pick = isPickable;
    transform.isAlive = true;
	smgr->addCubeNode(transform);
	
	transform.Position = glm::vec3(-4,0,0);
	transform.Scale = glm::vec3(Minier);
	transform.Color = glm::vec3(1);
    smgr->addSphereNode(transform);
	
	transform.Position = glm::vec3(-1,0,0);
	transform.Scale = glm::vec3(Minier,Minier,2*Minier);
	transform.Color = glm::vec3(0.1,0.8,0.5);
	smgr->addCylinderNode(transform);
	
	transform.Position = glm::vec3(2,0,0);
	transform.Color = glm::vec3(1.0,0.0,1.0);
	smgr->addConeNode(transform);
	
	transform.Position = glm::vec3(5,0,0);
	transform.FaceNum = 6;
	transform.Ratio = 0.5;
	transform.Color = glm::vec3(1.0,0.0,0.0);
	smgr->addFrustumNode(transform);
	
	transform.Position = glm::vec3(8,0,0);
	transform.FaceNum = 4;
	transform.Color = glm::vec3(1.0,1.0,0.0);
	smgr->addPyramidNode(transform);

	//Stone
	transform.Position = glm::vec3(-7,0,2.5);
	transform.Scale = glm::vec3(Minier);
	transform.Color = glm::vec3(1.0);
	smgr->addCubeNode(transform, true, smgr->StoneTex);
	
	transform.Position = glm::vec3(-4,0,2.5);
	transform.Scale = glm::vec3(Minier);
    smgr->addSphereNode(transform, true, smgr->StoneTex);
	
	transform.Position = glm::vec3(-1,0,2.5);
	transform.Scale = glm::vec3(Minier,Minier,2*Minier);
	transform.Ratio = 1;
	smgr->addCylinderNode(transform, true, smgr->StoneTex);
	
	transform.Position = glm::vec3(2,0,2.5);
	smgr->addConeNode(transform, true, smgr->StoneTex);
	
	transform.Position = glm::vec3(5,0,2.5);
	transform.FaceNum = 6;
	transform.Ratio = 0.5;
	smgr->addFrustumNode(transform, true, smgr->StoneTex);
	
	transform.Position = glm::vec3(8,0,2.5);
	transform.FaceNum = 4;
	smgr->addPyramidNode(transform, true, smgr->StoneTex);

	//Obsidian
	transform.Position = glm::vec3(-7,0,5);
	transform.Scale = glm::vec3(Minier);
	transform.Color = glm::vec3(1.0);
	smgr->addCubeNode(transform, true, smgr->ObsidianTex);
	
	transform.Position = glm::vec3(-4,0,5);
	transform.Scale = glm::vec3(Minier);
    smgr->addSphereNode(transform, true, smgr->ObsidianTex);
	
	transform.Position = glm::vec3(-1,0,5);
	transform.Scale = glm::vec3(Minier,Minier,2*Minier);
	transform.Ratio = 1;
	smgr->addCylinderNode(transform, true, smgr->ObsidianTex);
	
	transform.Position = glm::vec3(2,0,5);
	smgr->addConeNode(transform, true, smgr->ObsidianTex);
	
	transform.Position = glm::vec3(5,0,5);
	transform.FaceNum = 6;
	transform.Ratio = 0.5;
	smgr->addFrustumNode(transform, true, smgr->ObsidianTex);
	
	transform.Position = glm::vec3(8,0,5);
	transform.FaceNum = 4;
	smgr->addPyramidNode(transform, true, smgr->ObsidianTex);

	//Wood
	transform.Position = glm::vec3(-7,0,-2.5);
	transform.Scale = glm::vec3(Minier);
	transform.Color = glm::vec3(1.0);
	smgr->addCubeNode(transform, true, smgr->WoodTex);
	
	transform.Position = glm::vec3(-4,0,-2.5);
	transform.Scale = glm::vec3(Minier);
    smgr->addSphereNode(transform, true, smgr->WoodTex);
	
	transform.Position = glm::vec3(-1,0,-2.5);
	transform.Scale = glm::vec3(Minier,Minier,2*Minier);
	transform.Ratio = 1;
	smgr->addCylinderNode(transform, true, smgr->WoodTex);
	
	transform.Position = glm::vec3(2,0,-2.5);
	smgr->addConeNode(transform, true, smgr->WoodTex);
	
	transform.Position = glm::vec3(5,0,-2.5);
	transform.FaceNum = 6;
	transform.Ratio = 0.5;
	smgr->addFrustumNode(transform, true, smgr->WoodTex);
	
	transform.Position = glm::vec3(8,0,-2.5);
	transform.FaceNum = 4;
	smgr->addPyramidNode(transform, true, smgr->WoodTex);

	//Brick
	transform.Position = glm::vec3(-7,0,-5);
	transform.Scale = glm::vec3(Minier);
	transform.Color = glm::vec3(1.0);
	smgr->addCubeNode(transform, true, smgr->BrickTex);
	
	transform.Position = glm::vec3(-4,0,-5);
	transform.Scale = glm::vec3(Minier);
    smgr->addSphereNode(transform, true, smgr->BrickTex);
	
	transform.Position = glm::vec3(-1,0,-5);
	transform.Scale = glm::vec3(Minier,Minier,2*Minier);
	transform.Ratio = 1;
	smgr->addCylinderNode(transform, true, smgr->BrickTex);
	
	transform.Position = glm::vec3(2,0,-5);
	smgr->addConeNode(transform, true, smgr->BrickTex);
	
	transform.Position = glm::vec3(5,0,-5);
	transform.FaceNum = 6;
	transform.Ratio = 0.5;
	smgr->addFrustumNode(transform, true, smgr->BrickTex);
	
	transform.Position = glm::vec3(8,0,-5);
	transform.FaceNum = 4;
	smgr->addPyramidNode(transform, true, smgr->BrickTex);
	
	#ifndef __APPLE__
	smgr->addMeshSceneNode("../resource/nanosuit/nanosuit.obj");
	#else
	smgr->addMeshSceneNode("resource/nanosuit/nanosuit.obj");
	#endif
}

void Colorx::run()
{   
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        eventer->deltaTime = currentFrame - eventer->lastFrame;
        eventer->lastFrame = currentFrame;
        glfwGetWindowSize(smgr->window, &smgr->wWidth, &smgr->wHeight);
		processInput(window);
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        smgr->drawAll();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return;
}

Colorx::~Colorx(){

}