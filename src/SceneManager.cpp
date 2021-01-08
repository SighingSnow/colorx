/*
 * Function:
 * Camera / Light / Mesh is all in here
 */
#ifndef PI
#define PI 3.14159265358979323846f
#endif

#include "../include/SceneManager.h"

/*	Class SceneNode's constructor and draw(Shader &shader) function
 */
inline SceneNode::SceneNode(TYPE Type):
	pos				(glm::vec3(0.0, 0.0, 0.0)),
	rotAngle		(0),
	rotAxis			(glm::vec3(0.0, 0.0, 0.0)),
	scale			(glm::vec3(1.0, 1.0, 1.0))
{
	type			= Type;
}

inline SceneNode::SceneNode(glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, TYPE Type)
{
	pos				= Pos;
	rotAngle		= RotAngle;
	rotAxis			= RotAxis;
	scale			= Scale;
	type			= Type;
}

void SceneNode::draw(Shader &shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, this->pos);
	model = glm::rotate(model, glm::radians(this->rotAngle), this->rotAxis);
	model = glm::scale(model, this->scale);
	shader.setMat4("model", model);

	switch (this->type)
	{
		case _CUBE_:
		{
			glEnable(GL_CULL_FACE);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			glDisable(GL_CULL_FACE);
			break;
		}
		case _SPHERE_:
		{
			glEnable(GL_CULL_FACE);
			glDrawElements(GL_TRIANGLES, 60 * 60 * 6, GL_UNSIGNED_INT, 0);
			glDisable(GL_CULL_FACE);
			break;
		}
		case _CYLINDER_:
		{
			glEnable(GL_CULL_FACE);
			glDrawElements(GL_TRIANGLES, 100 * 4 * 3, GL_UNSIGNED_INT, 0);
			glDisable(GL_CULL_FACE);
			break;
		}
		case _CONE_:
		{
			glEnable(GL_CULL_FACE);
			glDrawElements(GL_TRIANGLES, 100 * 2 * 3, GL_UNSIGNED_INT, 0);
			glDisable(GL_CULL_FACE);
			break;
		}
		default:
		{
			break;
		}
	}
}


/*	Class SceneManager's public functions.
 *	setupSMGR() uses GenStdXXX() to build the vectors which save
 *	the coordinates of those standard polyhedra. And then bind 
 *	VAO, VBO and EBOs for the preparation of the render loop.
 */
void SceneManager::setupSMGR()
{
	//1. Set CUBE
	GenStdCube();
	glGenVertexArrays(1, &this->VAO[_CUBE_]);
	glGenBuffers(1, &this->VBO[_CUBE_]);

	glBindVertexArray(this->VAO[_CUBE_]);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[_CUBE_]);
	glBufferData(GL_ARRAY_BUFFER, this->StdCube_Vertex.size() * sizeof(float), &this->StdCube_Vertex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->EBO[_CUBE_]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO[_CUBE_]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->StdCube_Index.size() * sizeof(int), &this->StdCube_Index[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// color attribute
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	//2. Set SPHERE
	GenStdSphere();
	glGenVertexArrays(1, &this->VAO[_SPHERE_]);
	glGenBuffers(1, &this->VBO[_SPHERE_]);

	glBindVertexArray(this->VAO[_SPHERE_]);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[_SPHERE_]);
	glBufferData(GL_ARRAY_BUFFER, this->StdSphere_Vertex.size() * sizeof(float), &this->StdSphere_Vertex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->EBO[_SPHERE_]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO[_SPHERE_]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->StdSphere_Index.size() * sizeof(int), &this->StdSphere_Index[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// color attribute
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//3. Set CYLINDER
	GenStdCylinder();
	glGenVertexArrays(1, &this->VAO[_CYLINDER_]);
	glGenBuffers(1, &this->VBO[_CYLINDER_]);

	glBindVertexArray(this->VAO[_CYLINDER_]);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[_CYLINDER_]);
	glBufferData(GL_ARRAY_BUFFER, this->StdCylinder_Vertex.size() * sizeof(float), &this->StdCylinder_Vertex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->EBO[_CYLINDER_]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO[_CYLINDER_]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->StdCylinder_Index.size() * sizeof(int), &this->StdCylinder_Index[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// color attribute
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//4. Set CONE
	GenStdCone();
	glGenVertexArrays(1, &this->VAO[_CONE_]);
	glGenBuffers(1, &this->VBO[_CONE_]);

	glBindVertexArray(this->VAO[_CONE_]);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO[_CONE_]);
	glBufferData(GL_ARRAY_BUFFER, this->StdCone_Vertex.size() * sizeof(float), &this->StdCone_Vertex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->EBO[_CONE_]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO[_CONE_]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->StdCone_Index.size() * sizeof(int), &this->StdCone_Index[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// color attribute
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//	Delete buffers
void SceneManager::deleteSMGR()
{
	for (int i = 0; i < 7; i++)
	{
		glDeleteVertexArrays(1, &this->VAO[i]);
		glDeleteBuffers(1, &this->VBO[i]);
		glDeleteBuffers(1, &this->EBO[i]);
	}
}

//	Traversal every nodes in our SceneManager and draw them.
void SceneManager::drawAll(Shader &shader)
{
	for (unsigned int i = 0; i < this->commonNodes.size(); i++)
	{
		glBindVertexArray(this->VAO[this->commonNodes[i].type]);
		this->commonNodes[i].draw(shader);
	}
}


/*	The following functions add a certain node to our SceneManager.
 *	Each of them has two reload functions. 
 *	One requires no transformation parameters in input.
 *	The other one requires them.
 */
void SceneManager::addCubeNode(SceneManager *smgr, int id = 0)
{
	SceneNode::SceneNode Node(_CUBE_);

	this->commonNodes.push_back(Node);

	delete &Node;
}

void SceneManager::addCubeNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, int id = 0)
{
	SceneNode::SceneNode Node(Pos, RotAngle, RotAxis, Scale, _CUBE_);

	this->commonNodes.push_back(Node);

	delete &Node;
}

void SceneManager::addSphereNode(SceneManager *smgr, int id = 0)
{
	SceneNode::SceneNode Node(_SPHERE_);

	this->commonNodes.push_back(Node);

	delete &Node;
}

void SceneManager::addSphereNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, int id = 0)
{
	SceneNode::SceneNode Node(Pos, RotAngle, RotAxis, Scale, _SPHERE_);

	this->commonNodes.push_back(Node);

	delete &Node;
}

void SceneManager::addCylinderNode(SceneManager *smgr, int id = 0)
{
	SceneNode::SceneNode Node(_CYLINDER_);

	this->commonNodes.push_back(Node);

	delete &Node;
}

void SceneManager::addCylinderNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, int id = 0)
{
	SceneNode::SceneNode Node(Pos, RotAngle, RotAxis, Scale, _CYLINDER_);

	this->commonNodes.push_back(Node);

	delete &Node;
}

void SceneManager::addConeNode(SceneManager *smgr, int id = 0)
{
	SceneNode::SceneNode Node(_CONE_);

	this->commonNodes.push_back(Node);

	delete &Node;
}

void SceneManager::addConeNode(SceneManager *smgr, glm::vec3 Pos, float RotAngle, glm::vec3 RotAxis, glm::vec3 Scale, int id = 0)
{
	SceneNode::SceneNode Node(Pos, RotAngle, RotAxis, Scale, _CONE_);

	this->commonNodes.push_back(Node);

	delete &Node;
}

Camera* SceneManager::addFpsCameraNode(SceneManager *smgr)
{
	Camera Cam;

	return &Cam;
}

Camera* SceneManager::addFpsCameraNode(SceneManager *smgr, glm::vec3 Pos, glm::vec3 Up)
{
	Camera Cam(Pos, Up);

	return &Cam;
}

/*	The following functions are in the private area of class SceneManager.
 *	They are used once in the public function setSMGR() to build the vectors 
 *	which save the coordinates of the standard geometry polyhedra.
 */	
void SceneManager::GenStdCube()
{
	//Save the common coords of a cube
	float cube[8][3], texture[4][2];
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 3; j++)
		{
			cube[i][j] = -0.5f + (i >> (2-j) & 1);
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
			this->StdCube_Vertex.push_back(cube[v][0]);
			this->StdCube_Vertex.push_back(cube[v][1]);
			this->StdCube_Vertex.push_back(cube[v][2]);
			this->StdCube_Vertex.push_back(texture[j][0]);
			this->StdCube_Vertex.push_back(texture[j][1]);
		}
		this->StdCube_Index.push_back(4*i + 0);
		this->StdCube_Index.push_back(4*i + 2);
		this->StdCube_Index.push_back(4*i + 1);
		this->StdCube_Index.push_back(4*i + 1);
		this->StdCube_Index.push_back(4*i + 2);
		this->StdCube_Index.push_back(4*i + 3);
	}
}

void SceneManager::GenStdSphere()
{
	//Generate Vertices and this->StdSphere_Index
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
			this->StdSphere_Vertex.push_back(xPos);
			this->StdSphere_Vertex.push_back(yPos);
			this->StdSphere_Vertex.push_back(zPos);

			//Texture coords
			double u = std::atan2(zPos, xPos) / 2.0 / PI + 0.5;
			double v = std::acos(yPos) / PI;
			this->StdSphere_Vertex.push_back((float)u);
			this->StdSphere_Vertex.push_back((float)v);

			//this->StdSphere_Index
			if (j < 60 && i < 60)
			{
				this->StdSphere_Index.push_back(j * (60+1) + i);
				this->StdSphere_Index.push_back((j+1) * (60+1) + i);
				this->StdSphere_Index.push_back((j+1) * (60+1) + i+1);
				this->StdSphere_Index.push_back(j * (60+1) + i);
				this->StdSphere_Index.push_back((j+1) * (60+1) + i+1);
				this->StdSphere_Index.push_back(j * (60+1) + i+1);
			}
		}
	}
}

void SceneManager::GenStdCylinder()
{
	double Angle = 2 * PI / 100;
	int offset = 0;
	for (int i = 0; i <= 100; i++)
	{
		//Bottom circle
		this->StdCylinder_Vertex.push_back(std::cos(i * Angle));
		this->StdCylinder_Vertex.push_back(std::sin(i * Angle));
		this->StdCylinder_Vertex.push_back(0.0f);
		//Bottom texture coords
		this->StdCylinder_Vertex.push_back(i / 100);
		this->StdCylinder_Vertex.push_back(0.0f);

		//Top circle
		this->StdCylinder_Vertex.push_back(std::cos(i * Angle));
		this->StdCylinder_Vertex.push_back(std::sin(i * Angle));
		this->StdCylinder_Vertex.push_back(1.0f);
		//Top texture coords
		this->StdCylinder_Vertex.push_back(i / 100);
		this->StdCylinder_Vertex.push_back(1.0f);

		if (i < 100)
		{
			this->StdCylinder_Index.push_back(2*i);
			this->StdCylinder_Index.push_back(2*i + 2);
			this->StdCylinder_Index.push_back(2*i + 1);
			this->StdCylinder_Index.push_back(2*i + 1);
			this->StdCylinder_Index.push_back(2*i + 2);
			this->StdCylinder_Index.push_back(2*i + 3);
		}
		else
		{
			offset += (i+1)*2;
		}
	}

	//Bottom Center
	this->StdCone_Vertex.push_back(0.0);
	this->StdCone_Vertex.push_back(0.0);
	this->StdCone_Vertex.push_back(0.0);
	//Bottom Center texture coords
	this->StdCone_Vertex.push_back(0.5);
	this->StdCone_Vertex.push_back(0.5);

	offset++;

	for (int i = 0; i <= 100; i++)
	{
		//Bottom circle
		this->StdCone_Vertex.push_back(std::cos(i * Angle));
		this->StdCone_Vertex.push_back(std::sin(i * Angle));
		this->StdCone_Vertex.push_back(0.0f);
		//Texture coords
		this->StdCone_Vertex.push_back(std::cos(i * Angle) / 2 + 0.5);
		this->StdCone_Vertex.push_back(std::sin(i * Angle) / 2 + 0.5);

		if (i < 100)
		{
			this->StdCylinder_Index.push_back(offset+2 + i);
			this->StdCylinder_Index.push_back(offset+1 + i);
			this->StdCylinder_Index.push_back(offset);
		}
		else
		{
			offset += i+1;
		}
	}

	//Top Center
	this->StdCone_Vertex.push_back(0.0);
	this->StdCone_Vertex.push_back(0.0);
	this->StdCone_Vertex.push_back(1.0);
	//Top Center texture coords
	this->StdCone_Vertex.push_back(0.5);
	this->StdCone_Vertex.push_back(0.5);

	offset++;

	for (int i = 0; i <= 100; i++)
	{
		//Top circle
		this->StdCone_Vertex.push_back(std::cos(i * Angle));
		this->StdCone_Vertex.push_back(std::sin(i * Angle));
		this->StdCone_Vertex.push_back(1.0f);
		//Texture coords
		this->StdCone_Vertex.push_back(std::cos(i * Angle) / 2 + 0.5);
		this->StdCone_Vertex.push_back(std::sin(i * Angle) / 2 + 0.5);

		if (i < 100)
		{
			this->StdCylinder_Index.push_back(offset);
			this->StdCylinder_Index.push_back(offset+1 + i);
			this->StdCylinder_Index.push_back(offset+2 + i);
		}
	}
}

void SceneManager::GenStdCone()
{
	double Angle = 2 * PI / 100;
	int offset = 0;
	//Apex coords
	this->StdCone_Vertex.push_back(0.0);
	this->StdCone_Vertex.push_back(0.0);
	this->StdCone_Vertex.push_back(1.0);
	//Apex texture coords
	this->StdCone_Vertex.push_back(0.5);
	this->StdCone_Vertex.push_back(1.0);
	for (int i = 0; i <= 100; i++)
	{
		//Lateral surface
		this->StdCone_Vertex.push_back(std::cos(i * Angle));
		this->StdCone_Vertex.push_back(std::sin(i * Angle));
		this->StdCone_Vertex.push_back(0.0f);

		this->StdCone_Vertex.push_back(i / 100);
		this->StdCone_Vertex.push_back(0.0f);

		if (i < 100)
		{
			this->StdCylinder_Index.push_back(offset);
			this->StdCylinder_Index.push_back(offset+1 + i);
			this->StdCylinder_Index.push_back(offset+2 + i);
		}
		else
		{
			offset += i+1;
		}
	}

	//Bottom surface
	//Bottom Center
	this->StdCone_Vertex.push_back(0.0);
	this->StdCone_Vertex.push_back(0.0);
	this->StdCone_Vertex.push_back(0.0);
	//Center texture coords
	this->StdCone_Vertex.push_back(0.5);
	this->StdCone_Vertex.push_back(0.5);

	offset++;

	for (int i = 0; i <= 100; i++)
	{
		this->StdCone_Vertex.push_back(std::cos(i * Angle));
		this->StdCone_Vertex.push_back(std::sin(i * Angle));
		this->StdCone_Vertex.push_back(0.0f);

		this->StdCone_Vertex.push_back(std::cos(i * Angle) / 2 + 0.5);
		this->StdCone_Vertex.push_back(std::sin(i * Angle) / 2 + 0.5);

		if (i < 100)
		{
			this->StdCylinder_Index.push_back(offset+2 + i);
			this->StdCylinder_Index.push_back(offset+1 + i);
			this->StdCylinder_Index.push_back(offset);
		}
	}
}