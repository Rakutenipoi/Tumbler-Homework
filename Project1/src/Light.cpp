#include "../include/Light.h"

Light::Light(LIGHT_TYPE type)
{
	this->type = type;
	this->init(type);
}

void Light::init(LIGHT_TYPE type)
{
	switch (type)
	{
		case LIGHT_TYPE::POINT:
			this->constant = 1.0f;
			this->linear = 0.09f;
			this->quadratic = 0.032f;
			break;
		case LIGHT_TYPE::DIRECTION:

			break;
		case LIGHT_TYPE::SPOT:

			break;
		default:
			break;
	}
	this->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	this->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	this->specular = glm::vec3(0.1f, 0.1f, 0.1f);
}

void Light::setPosition(glm::vec3 position)
{
	this->position = position;
}

void Light::apply(Shader shader, Camera camera)
{
	switch (this->type)
	{
		case LIGHT_TYPE::POINT:
			shader.setFloat("pointLight.constant", this->constant);
			shader.setFloat("pointLight.linear", this->linear);
			shader.setFloat("pointLight.quadratic", this->quadratic);
			break;
		case LIGHT_TYPE::DIRECTION:

			break;
		case LIGHT_TYPE::SPOT:

			break;
		default:
			break;
	}
	shader.setVector3("pointLight.position", this->position);
	shader.setVector3("pointLight.ambient", this->ambient);
	shader.setVector3("pointLight.diffuse", this->diffuse);
	shader.setVector3("pointLight.specular", this->specular);
	shader.setVector3("viewPos", camera.Position);
}
