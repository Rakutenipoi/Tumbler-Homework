#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Model.h"

class PhysModel : public Model
{
public:
	PhysModel(glm::vec3 position, float mass, char* path);
	~PhysModel() {
		delete[] this->aabb;
	}
	glm::mat4 update(float deltaTime);

	void setPos(glm::vec3 value);
	void setVel(glm::vec3 value);
	void setAcc(glm::vec3 value);
	void setPosAngle(glm::vec3 value);
	void setVelAngle(glm::vec3 value);
	void setAccAngle(glm::vec3 value);
	void setFric(float value);
	
	glm::vec3 getPos() {
		return this->position;
	};

	glm::vec3 getPosAngle() {
		return this->position_angle;
	};

	AABB* aabb;
	bool stop;

protected:
	glm::vec3 position;
	glm::vec3 position_angle;
	glm::vec3 velocity;
	glm::vec3 velocity_angle;
	glm::vec3 acceleration;
	glm::vec3 acceleration_angle;

	float mass;
	float friction;
	float friction_angle;
	float torque_length;
};

