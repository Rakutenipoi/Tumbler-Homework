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
#include <ctime>
#include <random>
#include <glm/gtc/random.hpp>

#include "Light.h"

namespace display{
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void processInput(GLFWwindow* window);
}

void Display(GLFWwindow* window);
void InitSphere();
float generateRandomNumber();
std::vector<glm::mat4> configLightMatrix(Light light);


