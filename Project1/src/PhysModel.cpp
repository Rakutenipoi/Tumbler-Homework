#include "../include/PhysModel.h"

PhysModel::PhysModel(glm::vec3 position, float mass, char* path)
{
	this->loadModel(path);
	this->aabb = new AABB(glm::vec3(-0.1f), glm::vec3(0.1f, 0.215f, 0.1f));
	this->stop = false;
	this->position = position;
	this->velocity = glm::vec3(0.0f);
	this->acceleration = glm::vec3(0.0f);
}

glm::mat4 PhysModel::update(float deltaTime)
{


	return glm::translate(glm::mat4(1.0f), this->position);
}

void PhysModel::setPos(glm::vec3 value)
{
	this->position = value;
}

void PhysModel::setVel(glm::vec3 value)
{
	this->velocity = value;
}

void PhysModel::setAcc(glm::vec3 value)
{
	this->acceleration = value;
}
