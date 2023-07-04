#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Shader.h"

enum MATRIX_TYPE
{
	MODEL,
	VIEW,
	PROJECTION,
};

class Plane
{
public:
	Plane();
	void setShader(Shader shader);
	void setMatrix(MATRIX_TYPE type, glm::mat4 value);
	void setMatrix(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	void setData(float* vertices, unsigned int* indices, int numVertices, int numIndices);
	void draw(glm::vec3 color);

private:
	float* vertices;
	unsigned int* indices;
	unsigned int vbo, vao, ebo;

	Shader shader;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

};

