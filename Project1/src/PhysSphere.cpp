#include "../include/PhysSphere.h"

PhysSphere::PhysSphere(glm::vec3 position, float radius, int slices, int stacks)
{
	this->init(radius, slices, stacks);
	this->position = position;
	this->velocity = glm::vec3(0.0f);
	this->acceleration = glm::vec3(0.0f);
	this->model = glm::translate(glm::mat4(1.0f), this->position);
}

glm::mat4 PhysSphere::update(float deltaTime)
{
	this->velocity += deltaTime * this->acceleration;
	this->position += deltaTime * this->velocity;

	// this->model = glm::translate(glm::mat4(1.0f), this->position);
	return glm::translate(glm::mat4(1.0f), this->position);
}

void PhysSphere::setPos(glm::vec3 value)
{
	this->position = value;
}

void PhysSphere::setAcc(glm::vec3 value)
{
	this->acceleration = value;
}




