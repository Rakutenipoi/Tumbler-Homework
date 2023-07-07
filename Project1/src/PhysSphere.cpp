#include "../include/PhysSphere.h"

PhysSphere::PhysSphere(glm::vec3 position, float radius, int slices, int stacks, float mass)
{
	this->stop = false;
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

	// ±ß½çÖµ
	float bound = 0.5f - this->radius;

	if (this->stop && this->position.y <= -bound) {
		this->color = glm::vec3(0.0f);
		this->alpha = 0.0f;
	}

	if (!this->stop) {
		if (abs(this->position.x) > bound) {
			this->position.x = bound * this->position.x / abs(this->position.x);
			this->velocity.x *= -1;
		}
		else if (abs(this->position.y) > bound) {
			this->position.y = bound * this->position.y / abs(this->position.y);
			this->velocity.y *= -1;
		}
		else if (abs(this->position.z) > bound) {
			this->position.z = bound * this->position.z / abs(this->position.z);
			this->velocity.z *= -1;
		}
	}
	

	// this->model = glm::translate(glm::mat4(1.0f), this->position);
	return glm::translate(glm::mat4(1.0f), this->position);
}

void PhysSphere::setPos(glm::vec3 value)
{
	this->position = value;
}

void PhysSphere::setVel(glm::vec3 value)
{
	this->velocity = value;
}

void PhysSphere::setAcc(glm::vec3 value)
{
	this->acceleration = value;
}




