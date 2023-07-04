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
#include "Plane.h"

class Box : public Plane
{
public:
	Box();
	void setData(float* vertices, unsigned int* indices, int numVertices, int numIndices);
	void setData();
	void draw();

};

