#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Plane.h"
#include "Shader.h"

using namespace std;

class Sphere : public Plane
{
public:
	Sphere();
	Sphere(float radius, int slices, int stacks); 
	void setColor(glm::vec3 color);
	void draw();
	void init(float radius, int slices, int stacks);
	void setAlpha(float value);

	float getRadius() {
		return this->radius;
	}

protected:
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<unsigned int> indices;
	glm::vec3 color;
	float alpha;
	float radius;
	int slices;
	int stacks;

	void createSphere(float radius, int slices, int stacks);
};

