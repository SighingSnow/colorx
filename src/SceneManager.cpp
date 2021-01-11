/*
 * Function:
 * Camera / Light / Mesh is all in here
 */


#include "../include/SceneManager.h"

#include "svpng.h"

/*	Class SceneNode's constructor and draw(Shader &shader) function
 */
/* This is just a test */
void SceneManager::addMeshSceneNode(SceneManager *smgr, const char* path ,int id)
{
	Model ourModel("./resource/nanosuit/nanosuit.obj");
	meshNodes.push_back(ourModel);
}

void SceneNode::draw(Shader &shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, rotAngle, rotAxis);
	model = glm::scale(model, scale);
	model = glm::translate(model, pos);
	model = glm::scale(model, glm::vec3(0.4f));
    shader.setMat4("model", model);
	shader.setVec3("objectColor",color);
	switch (this->type)
	{
		case _CUBE_:
		{
			glEnable(GL_CULL_FACE);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glDisable(GL_CULL_FACE);
			break;
		}
		case _SPHERE_:
		{
			glEnable(GL_CULL_FACE);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glDisable(GL_CULL_FACE);
			break;
		}
		case _CYLINDER_:
		{
			glEnable(GL_CULL_FACE);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glDisable(GL_CULL_FACE);
			break;
		}
		case _CONE_:
		{
			glEnable(GL_CULL_FACE);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glDisable(GL_CULL_FACE);
			break;
		}
		default:
		{
			break;
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


//	Traversal every nodes in our SceneManager and draw them.
void SceneManager::drawAll()
{
	for (unsigned int i = 0; i < this->commonNodes.size(); i++)
	{
		this->commonNodes[i].draw(*(this->commonShader));
	}
	for(unsigned int i = 0; i < this->meshNodes.size(); i++){
		this->meshNodes[i].Draw(*(this->meshShader));
	}
}


/*	The following functions add a certain node to our SceneManager.
 *	Each of them has two reload functions. 
 *	One requires no transformation parameters in input.
 *	The other one requires them.
 */
void SceneManager::addCubeNode(SceneManager *smgr, int id)
{
	
	SceneNode* Node = new SceneNode(_CUBE_);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addCubeNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, glm::vec3 Color ,int id)
{
	SceneNode* Node = new SceneNode(Pos, RotAngle, RotAxis, Scale,Color ,_CUBE_,id);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addSphereNode(SceneManager *smgr, int id)
{
	SceneNode* Node = new SceneNode(_SPHERE_);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addSphereNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale,glm::vec3 Color , int id)
{
	SceneNode* Node = new SceneNode(Pos, RotAngle, RotAxis, Scale, Color,_SPHERE_,id);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addCylinderNode(SceneManager *smgr, int id)
{
	SceneNode* Node = new SceneNode(_CYLINDER_);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addCylinderNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale,glm::vec3 Color , int id)
{
	SceneNode* Node = new SceneNode(Pos, RotAngle, RotAxis, Scale,Color, _CYLINDER_,id);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addConeNode(SceneManager *smgr, int id)
{
	SceneNode* Node = new SceneNode(_CONE_);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addConeNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale,glm::vec3 Color , int id)
{
	SceneNode* Node = new SceneNode(Pos, RotAngle, RotAxis, Scale, Color ,_CONE_,id);

	this->commonNodes.push_back(*Node);

	delete Node;
}

/*	The following functions are in the private area of class SceneManager.
 *	They are used once in the public function setSMGR() to build the vectors 
 *	which save the coordinates of the standard geometry polyhedra.
 */	
void SceneNode::GenStdCube()
{
	//Save the common coords of a cube
	float cube[8][3], normal[6][3] = {0.0f}, texture[4][2];
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 3; j++)
		{
			cube[i][j] = -0.5f + (i >> (2-j) & 1);
		}
	for (int i = 0; i < 6; i++)
	{
		int k = i%3, h = i&1;
		normal[i][2-k] = 2*((float)h - 0.5f);
	}
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 2; j++)
		{
			texture[i][j] = i >> (1-j) & 1;
		}

	//Generate standard cube coords from above
	for (int i = 0; i < 6; i++)
	{
		int k = i%3, h = (i&1)<<k;
		for (int j = 0; j < 4; j++)
		{
			int a = j>>1, b = j&1, v;
			k==1 ? v=(a<<2)+h+b : v=(j<<((k+1)%3))+h;
			this->vertices.push_back(cube[v][0]);
			this->vertices.push_back(cube[v][1]);
			this->vertices.push_back(cube[v][2]);
			this->vertices.push_back(normal[i][0]);
			this->vertices.push_back(normal[i][1]);
			this->vertices.push_back(normal[i][2]);
			this->vertices.push_back(texture[j][0]);
			this->vertices.push_back(texture[j][1]);
		}
		if(i < 3){
			this->indices.push_back(4*i + 0);
			this->indices.push_back(4*i + 1);
			this->indices.push_back(4*i + 2);
			this->indices.push_back(4*i + 1);
			this->indices.push_back(4*i + 3);
			this->indices.push_back(4*i + 2);
		}
		else{
			this->indices.push_back(4*i + 0);
			this->indices.push_back(4*i + 2);
			this->indices.push_back(4*i + 1);
			this->indices.push_back(4*i + 1);
			this->indices.push_back(4*i + 2);
			this->indices.push_back(4*i + 3);
		}
	}
	
}

void SceneNode::GenStdSphere()
{
	//Generate Vertices and this->indices
	for (int j = 0; j <= 60; j++)
	{
		for (int i = 0; i <= 60; i++)
		{
			//Vertices
			float xSegment = (float)i / (float)60;
			float ySegment = (float)j / (float)60;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			this->vertices.push_back(xPos);
			this->vertices.push_back(yPos);
			this->vertices.push_back(zPos);

			//Normals
			this->vertices.push_back(xPos);
			this->vertices.push_back(yPos);
			this->vertices.push_back(zPos);

			//Texture coords
			double u = std::atan2(zPos, xPos) / 2.0 / PI + 0.5;
			double v = std::acos(yPos) / PI;
			this->vertices.push_back((float)u);
			this->vertices.push_back((float)v);

			//this->indices
			if (j < 60 && i < 60)
			{
				this->indices.push_back(j * (60+1) + i);
				this->indices.push_back((j+1) * (60+1) + i);
				this->indices.push_back((j+1) * (60+1) + i+1);
				this->indices.push_back(j * (60+1) + i);
				this->indices.push_back((j+1) * (60+1) + i+1);
				this->indices.push_back(j * (60+1) + i+1);
			}
		}
	}
}

void SceneNode::GenStdCylinder()
{
	double Angle = 2 * PI / 100;
	int offset = 0;
	for (int i = 0; i <= 100; i++)
	{
		//Bottom circle
		this->vertices.push_back(std::cos(i * Angle));
		this->vertices.push_back(std::sin(i * Angle));
		this->vertices.push_back(-0.5f);
		//Bottom normals
		this->vertices.push_back(std::cos(i * Angle));
		this->vertices.push_back(std::sin(i * Angle));
		this->vertices.push_back(0.0f);
		//Bottom texture coords
		this->vertices.push_back(i / 100);
		this->vertices.push_back(0.0f);

		//Top circle
		this->vertices.push_back(std::cos(i * Angle));
		this->vertices.push_back(std::sin(i * Angle));
		this->vertices.push_back(0.5f);
		//Top normals
		this->vertices.push_back(std::cos(i * Angle));
		this->vertices.push_back(std::sin(i * Angle));
		this->vertices.push_back(0.0f);
		//Top texture coords
		this->vertices.push_back(i / 100);
		this->vertices.push_back(1.0f);

		if (i < 100)
		{
			this->indices.push_back(2*i);
			this->indices.push_back(2*i + 2);
			this->indices.push_back(2*i + 1);
			this->indices.push_back(2*i + 1);
			this->indices.push_back(2*i + 2);
			this->indices.push_back(2*i + 3);
		}
		else
		{
			offset += (i+1)*2;
		}
	}

	//Bottom Center
	this->vertices.push_back(0.0);
	this->vertices.push_back(0.0);
	this->vertices.push_back(-0.5);
	//Bottom Center normal
	this->vertices.push_back(0.0);
	this->vertices.push_back(0.0);
	this->vertices.push_back(-1.0);
	//Bottom Center texture coords
	this->vertices.push_back(0.5);
	this->vertices.push_back(0.5);

	offset++;

	for (int i = 0; i <= 100; i++)
	{
		//Bottom circle
		this->vertices.push_back(std::cos(i * Angle));
		this->vertices.push_back(std::sin(i * Angle));
		this->vertices.push_back(-0.5f);
		//Bottom circle normal
		this->vertices.push_back(0.0);
		this->vertices.push_back(0.0);
		this->vertices.push_back(-1.0);
		//Texture coords
		this->vertices.push_back(std::cos(i * Angle) / 2 + 0.5);
		this->vertices.push_back(std::sin(i * Angle) / 2 + 0.5);

		if (i < 100)
		{
			this->indices.push_back(offset+2 + i);
			this->indices.push_back(offset+1 + i);
			this->indices.push_back(offset);
		}
		else
		{
			offset += i+1;
		}
	}

	//Top Center
	this->vertices.push_back(0.0);
	this->vertices.push_back(0.0);
	this->vertices.push_back(0.5);
	//Top Center normal
	this->vertices.push_back(0.0);
	this->vertices.push_back(0.0);
	this->vertices.push_back(1.0);
	//Top Center texture coords
	this->vertices.push_back(0.5);
	this->vertices.push_back(0.5);

	offset++;

	for (int i = 0; i <= 100; i++)
	{
		//Top circle
		this->vertices.push_back(std::cos(i * Angle));
		this->vertices.push_back(std::sin(i * Angle));
		this->vertices.push_back(0.5f);
		//Top circle normal
		this->vertices.push_back(0.0);
		this->vertices.push_back(0.0);
		this->vertices.push_back(1.0);
		//Texture coords
		this->vertices.push_back(std::cos(i * Angle) / 2 + 0.5);
		this->vertices.push_back(std::sin(i * Angle) / 2 + 0.5);

		if (i < 100)
		{
			this->indices.push_back(offset);
			this->indices.push_back(offset+1 + i);
			this->indices.push_back(offset+2 + i);
		}
	}
}

void SceneNode::GenStdCone()
{
	double Angle = 2 * PI / 100;
	int offset = 0;
	//Apex coords
	this->vertices.push_back(0.0);
	this->vertices.push_back(0.0);
	this->vertices.push_back(0.5);
	//Apex normal
	this->vertices.push_back(0.0);
	this->vertices.push_back(0.0);
	this->vertices.push_back(1.0);
	//Apex texture coords
	this->vertices.push_back(0.5);
	this->vertices.push_back(1.0);
	for (int i = 0; i <= 100; i++)
	{
		//Lateral surface
		this->vertices.push_back(std::cos(i * Angle));
		this->vertices.push_back(std::sin(i * Angle));
		this->vertices.push_back(-0.5f);
		//Lateral normals
		this->vertices.push_back(std::cos(i * Angle));
		this->vertices.push_back(std::sin(i * Angle));
		this->vertices.push_back(1.0f);
		//Lateral texture coords
		this->vertices.push_back(i / 100);
		this->vertices.push_back(0.0f);

		if (i < 100)
		{
			this->indices.push_back(offset);
			this->indices.push_back(offset+1 + i);
			this->indices.push_back(offset+2 + i);
		}
		else
		{
			offset += i+1;
		}
	}

	//Bottom surface
	//Bottom Center
	this->vertices.push_back(0.0);
	this->vertices.push_back(0.0);
	this->vertices.push_back(-0.5);
	//Bottom Center normal
	this->vertices.push_back(0.0);
	this->vertices.push_back(0.0);
	this->vertices.push_back(-1.0);
	//Center texture coords
	this->vertices.push_back(0.5);
	this->vertices.push_back(0.5);

	offset++;

	for (int i = 0; i <= 100; i++)
	{
		this->vertices.push_back(std::cos(i * Angle));
		this->vertices.push_back(std::sin(i * Angle));
		this->vertices.push_back(-0.5f);

		this->vertices.push_back(0.0);
		this->vertices.push_back(0.0);
		this->vertices.push_back(-1.0);

		this->vertices.push_back(std::cos(i * Angle) / 2 + 0.5);
		this->vertices.push_back(std::sin(i * Angle) / 2 + 0.5);

		if (i < 100)
		{
			this->indices.push_back(offset+2 + i);
			this->indices.push_back(offset+1 + i);
			this->indices.push_back(offset);
		}
	}
}


void SceneManager::prtScreen()
{
	int wWidth, wHeight;
	glfwGetWindowSize(window, &wWidth, &wHeight);

	time_t rawtime;
	time(&rawtime);
	
	std::ostringstream os;
    os<<rawtime;
    std::string pngname;
    std::istringstream is(os.str());
    is>>pngname;
	
	pngname += ".png";
	
	GLubyte pixels[wHeight][wWidth][3] ;
	memset(pixels,0,wHeight*wWidth*3*sizeof(GLubyte));

	glFlush(); glFinish();

	glReadPixels(0, 0, wWidth, wHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	for (int i = 0; i < wHeight/2; i++)
		for (int j = 0; j < wWidth; j++)
			for (int k = 0; k < 3; k++)
			{
				GLubyte temp = pixels[i][j][k];
				pixels[i][j][k] = pixels[wHeight-i-1][j][k];
				pixels[wHeight-i-1][j][k] = temp;
			}

    FILE *fp = fopen(pngname.c_str(), "wb+");
    svpng(fp, wWidth, wHeight, &pixels[0][0][0], 0);
    fclose(fp);

	free(pixels);
}