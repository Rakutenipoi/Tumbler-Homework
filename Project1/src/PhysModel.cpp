#include "../include/PhysModel.h"

#define G 0.96f
#define FRICTION_ANGLE 0.5f

PhysModel::PhysModel(glm::vec3 position, float mass, char* path)
{
	this->loadModel(path);
	this->aabb = new AABB(glm::vec3(-0.1f), glm::vec3(0.1f, 0.215f, 0.1f));
	this->stop = false;
	this->position = position;
	this->velocity = glm::vec3(0.0f);
	this->acceleration = glm::vec3(0.0f);
	this->friction_angle = FRICTION_ANGLE;
}

glm::mat4 PhysModel::update(float deltaTime)
{
	this->velocity += deltaTime * this->acceleration;
	this->position += deltaTime * this->velocity;
	this->velocity_angle += deltaTime * this->acceleration_angle;
	this->position_angle += deltaTime * this->velocity_angle;

	// ±ß½çÖµ
	float bound = 0.45f;

	// ±ß½çÅö×²ÅÐ¶Ï
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

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, this->position);
	model = glm::rotate(model, glm::radians(this->position_angle.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(this->position_angle.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(this->position_angle.z), glm::vec3(0.0f, 0.0f, 1.0f));
	
	return model;
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

void PhysModel::setPosAngle(glm::vec3 value)
{
	this->position_angle = value;
}

void PhysModel::setVelAngle(glm::vec3 value)
{
	this->velocity_angle = value;
}

void PhysModel::setAccAngle(glm::vec3 value)
{
	this->acceleration_angle = value;
}

void PhysModel::setFric(float value)
{
	this->friction = value;
}
