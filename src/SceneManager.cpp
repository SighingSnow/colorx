/*
 * Function:
 * Camera / Light / Mesh is all in here
 */


#include "../include/SceneManager.h"

#include "svpng.h"

/*	Class SceneNode's constructor and draw(Shader &shader) function
 */
/* This is just a test */
void SceneManager::addMeshSceneNode(SceneManager *smgr, const char* path)
{
	Model ourModel("./resource/nanosuit/nanosuit.obj");
	meshNodes.push_back(ourModel);
}

void SceneNode::draw(Shader &shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	/* Note if you change the model matrix here, the matrix in button_callback should also be modified. */
	model = glm::rotate(model, NodeAttr.RotAngle, NodeAttr.RotAxis);
	model = glm::translate(model, NodeAttr.Position * glm::vec3(Minier));
	model = glm::scale(model, NodeAttr.Scale);

    shader.setMat4("model", model);
	shader.setVec3("objectColor", NodeAttr.Color);

	shader.setBool("EnableTexture", false);
	if(texture){
		shader.setBool("EnableTexture", true);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	glEnable(GL_CULL_FACE);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glDisable(GL_CULL_FACE);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


//	Traversal every nodes in our SceneManager and draw them.
void SceneManager::drawAll()
{
	if(wire)	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (unsigned int i = 0; i < this->commonNodes.size() ; i++)
	{
		if(commonNodes[i].NodeAttr.isAlive == true){
			this->commonNodes[i].draw(*(this->commonShader));
		}
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
void SceneManager::addCubeNode(SceneManager *smgr, bool Texture, unsigned int TextureID)
{
	
	SceneNode* Node = new SceneNode(_CUBE_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addCubeNode(SceneManager *smgr, transAttr transform, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(transform, _CUBE_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addSphereNode(SceneManager *smgr, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(_SPHERE_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addSphereNode(SceneManager *smgr, transAttr transform, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(transform, _SPHERE_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addCylinderNode(SceneManager *smgr, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(_CYLINDER_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addCylinderNode(SceneManager *smgr, transAttr transform, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(transform, _CYLINDER_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addConeNode(SceneManager *smgr, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(_CONE_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addConeNode(SceneManager *smgr, transAttr transform, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(transform, _CONE_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addFrustumNode(SceneManager *smgr, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(_FRUSTUM_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addFrustumNode(SceneManager *smgr, transAttr transform, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(transform, _FRUSTUM_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addPyramidNode(SceneManager *smgr, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(_PYRAMID_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addPyramidNode(SceneManager *smgr, transAttr transform, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(transform, _PYRAMID_, Texture, TextureID);

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
			double u = xSegment;
			double v = ySegment;
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

void SceneNode::GenStdFrustum()
{
	int fN = NodeAttr.FaceNum;
	float r = NodeAttr.Ratio;
	double Angle = 2 * PI / fN;
	int offset = 0;

	for (int i = 0; i <= fN; i++)
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
		this->vertices.push_back((float)i / (float)fN);
		this->vertices.push_back(0.0f);

		//Top circle
		this->vertices.push_back(std::cos(i * Angle) * r);
		this->vertices.push_back(std::sin(i * Angle) * r);
		this->vertices.push_back(0.5f);
		//Top normals
		this->vertices.push_back(std::cos(i * Angle));
		this->vertices.push_back(std::sin(i * Angle));
		this->vertices.push_back(1.0f - r);
		//Top texture coords
		this->vertices.push_back((float)i / (float)fN);
		this->vertices.push_back(1.0f);

		if (i < fN)
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
			offset += (i+1) * 2;
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

	for (int i = 0; i <= fN; i++)
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

		if (i < fN)
		{
			this->indices.push_back(offset+1 + i);
			this->indices.push_back(offset   + i);
			this->indices.push_back(offset-1);
		}
		else
		{
			offset += i + 1;
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

	for (int i = 0; i <= fN; i++)
	{
		//Top circle
		this->vertices.push_back(std::cos(i * Angle) * r);
		this->vertices.push_back(std::sin(i * Angle) * r);
		this->vertices.push_back(0.5f);
		//Top circle normal
		this->vertices.push_back(0.0);
		this->vertices.push_back(0.0);
		this->vertices.push_back(1.0);
		//Texture coords
		this->vertices.push_back(std::cos(i * Angle) / 2 + 0.5);
		this->vertices.push_back(std::sin(i * Angle) / 2 + 0.5);

		if (i < fN)
		{
			this->indices.push_back(offset-1);
			this->indices.push_back(offset   + i);
			this->indices.push_back(offset+1 + i);
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

/* imply Trumbore algorithm */
bool SceneManager::intersect(glm::vec3 camPos,glm::vec3 camDir,glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,float& t,float& u,float& v)
{
	const float episilon = 0.00000001;
	glm::vec3 e1 = p2 - p1;
	glm::vec3 e2 = p3 - p1;
	glm::vec3 s = camPos - p1;
	glm::vec3 s1 = glm::cross(camDir,e2);
	glm::vec3 s2 = glm::cross(s,e1);

	float det = glm::dot(s1,e1);
	float detT = glm::dot(s2,e2);
	float detU = glm::dot(s1,s);
	float detV = glm::dot(s2,camDir);
	
	if(det < episilon && det>-episilon) return false;
	t = detT *(1.0/det);
	u = detU *(1.0/det);
	v = detV *(1.0/det);

	if(u < 0 || u > 1) return false;
	if(v < 0 || v > 1) return false;
	if(u+v>1) return false;
	if(t > episilon) return true;

	return false;
}

bool SceneManager::ifCollision(glm::vec3 nxtPos){
	for (unsigned int i = 0; i < this->commonNodes.size(); i++)
	{
		if(this->commonNodes[i].ifCollision(nxtPos)){
			return true;
		}
	}
	/*
	for(unsigned int i = 0; i < this->meshNodes.size(); i++){
		if(!this->meshNodes[i].ifCollision(nxtPos)){
			return false;
		}
	}
	*/
	return false;
}

bool SceneNode::ifCollision(glm::vec3 nxtPos){
	if(nxtPos[0]>NodeAttr.Position[0]-0.5*NodeAttr.Scale[0] && nxtPos[0]<NodeAttr.Position[0]+0.5*NodeAttr.Scale[0] &&
		nxtPos[1]>NodeAttr.Position[1]-0.5*NodeAttr.Scale[1] && nxtPos[1]<NodeAttr.Position[1]+0.5*NodeAttr.Scale[1] &&
		nxtPos[2]>NodeAttr.Position[2]-0.5*NodeAttr.Scale[2] && nxtPos[2]<NodeAttr.Position[2]+0.5*NodeAttr.Scale[2]
	)
		return true;
	return false;
}

unsigned int SceneManager::loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}