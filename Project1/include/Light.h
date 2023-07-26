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
#include "Camera.h"

enum class LIGHT_TYPE
{
	POINT, // 点光源
	DIRECTION, // 平行光
	SPOT, // 聚光
};

class Light
{
public:
	Light(LIGHT_TYPE type);
	~Light();
	void init(LIGHT_TYPE type);
	void setPosition(glm::vec3 position);
	void apply(Shader shader, Camera camera);
	void shadow(unsigned int width, unsigned int height);

	LIGHT_TYPE type;

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;

	unsigned int fbo;
};

