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

	AABB* aabb;
	bool stop;

protected:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float mass;
};

