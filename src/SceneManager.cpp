/*
 * Function:
 * Camera / Light / Mesh is all in here
 */


#include "../include/SceneManager.h"

#include "svpng.h"

/*	Class SceneNode's constructor and draw(Shader &shader) function
 */
/* This is just a test */
void SceneManager::addMeshSceneNode(const char* path)
{
	Model ourModel(path);
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

void Skybox::draw()
{
	glDisable(GL_CULL_FACE);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void Cross::draw()
{
	glDisable(GL_CULL_FACE);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

//	Traversal every nodes in our SceneManager and draw them.
void SceneManager::drawAll()
{
	if(wire)	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Traversal commonNodes
	commonShader->use();
	commonShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	commonShader->setVec3("lightPos", 1.2f, 1.0f, 3.0f);
	commonShader->setVec3("viewPos", camera->Position);
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)wWidth / wHeight, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	commonShader->setMat4("projection", projection);
	commonShader->setMat4("view", view);
	for (unsigned int i = 0; i < this->commonNodes.size() ; i++)
	{
		if(commonNodes[i].NodeAttr.isAlive == true){
			this->commonNodes[i].draw(*(this->commonShader));
		}
	}

	//Traversal meshNodes
	meshShader->use();
	meshShader->setMat4("projection", projection);
	meshShader->setMat4("view", view);
	glm::mat4 model = glm::mat4(1.0);
	meshShader->setMat4("model", model);
	for(unsigned int i = 0; i < this->meshNodes.size(); i++){
		this->meshNodes[i].Draw(*(this->meshShader));
	}

	//draw Skybox
	glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader->use();
	view = glm::mat4(glm::mat3(camera->GetViewMatrix())); // remove translation from the view matrix
	skyboxShader->setMat4("view", view);
	skyboxShader->setMat4("projection", projection);
	this->ourSkybox.draw();
	
	//draw cross
	glDepthFunc(GL_ALWAYS);
	crossShader->use();
	glm::vec2 scrCoeff = glm::vec2((50.0f / (float)wWidth), (50.0f / (float)wHeight));
	crossShader->setVec2("scrCoeff", scrCoeff);
	ourCross.draw();
}


/*	The following functions add a certain node to our SceneManager.
 *	Each of them has two reload functions. 
 *	One requires no transformation parameters in input.
 *	The other one requires them.
 */
void SceneManager::addCubeNode(bool Texture, unsigned int TextureID)
{
	
	SceneNode* Node = new SceneNode(_CUBE_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addCubeNode(transAttr transform, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(transform, _CUBE_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addSphereNode(bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(_SPHERE_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addSphereNode(transAttr transform, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(transform, _SPHERE_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addCylinderNode(bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(_CYLINDER_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addCylinderNode(transAttr transform, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(transform, _CYLINDER_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addConeNode(bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(_CONE_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addConeNode(transAttr transform, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(transform, _CONE_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addFrustumNode(bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(_FRUSTUM_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addFrustumNode(transAttr transform, bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(transform, _FRUSTUM_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addPyramidNode(bool Texture, unsigned int TextureID)
{
	SceneNode* Node = new SceneNode(_PYRAMID_, Texture, TextureID);

	this->commonNodes.push_back(*Node);

	delete Node;
}

void SceneManager::addPyramidNode(transAttr transform, bool Texture, unsigned int TextureID)
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
			float Theta = (float)i / (float)60 * 2.0f * PI;
			float Psi = (float)j / (float)60 * PI;
			float xPos = std::cos(Theta) * std::sin(Psi);
			float yPos = std::sin(Theta) * std::sin(Psi);
			float zPos = std::cos(Psi);
			this->vertices.push_back(xPos);
			this->vertices.push_back(yPos);
			this->vertices.push_back(zPos);

			//Normals
			this->vertices.push_back(xPos);
			this->vertices.push_back(yPos);
			this->vertices.push_back(zPos);

			//Texture coords
			float u = Theta / 2.0f / PI;
			float v = Psi / PI;
			this->vertices.push_back(u);
			this->vertices.push_back(v);

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
	time_t rawtime;
	time(&rawtime);
	
	std::ostringstream os;
    os<<rawtime;
    std::string pngname;
    std::istringstream is(os.str());
    is>>pngname;
	
	pngname += ".png";
	
	GLubyte *pixels;
	pixels = new GLubyte[wHeight*wWidth*3];
	memset(pixels,0,wHeight*wWidth*3*sizeof(GLubyte));

	glFlush(); glFinish();

	glReadPixels(0, 0, wWidth, wHeight, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0]);

	for (int i = 0; i < wHeight/2; i++)
		for (int j = 0; j < wWidth; j++)
			for (int k = 0; k < 3; k++)
			{
				GLubyte temp = pixels[i*wWidth*3 + j*3 + k];
				pixels[i*wWidth*3 + j*3 + k] = pixels[(wHeight-i-1)*wWidth*3 + j*3 + k];
				pixels[(wHeight-i-1)*wWidth*3 + j*3 + k] = temp;
			}

    FILE *fp = fopen(pngname.c_str(), "wb+");
    svpng(fp, wWidth, wHeight, &pixels[0], 0);
    fclose(fp);

	delete []pixels;
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

unsigned int SceneManager::loadTexture(std::string path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
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

unsigned int SceneManager::loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}