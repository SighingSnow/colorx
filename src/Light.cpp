#include "Light.h"


void Light::setLight(Shader* lightShader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
	lightShader->setVec3("light.diffuse", diffuse);
	lightShader->setVec3("light.ambient", ambient);
	lightShader->setVec3("light.specular", specular);
}

void Light::drawLight(Shader* lightShader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
	lightShader->use();

	setLight(lightShader, ambient, diffuse, specular);
	lightShader->setVec3("light.lightPos", this->position);
	lightShader->setFloat("strength", this->strength);
}

void Light::drawDirectionLight(Shader* lightShader, glm::vec3 direction) {
	lightShader->use();

	setLight(lightShader, this->ambient, this->diffuse, this->specular);
	lightShader->setVec3("light.direction", direction);
	lightShader->setFloat("strength", this->strength);
}


void Light::drawPointLight(Shader* lightShader, glm::vec3 direction) {
	lightShader->use();

	setLight(lightShader, this->ambient, this->diffuse, this->specular);
	lightShader->setFloat("light.constant", this->constant);
	lightShader->setFloat("light.linear", this->linear);
	lightShader->setFloat("light.quadratic", this->quadratic);
	lightShader->setFloat("strength", this->strength);
}