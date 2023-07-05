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

namespace edit{
	void Edit(GLFWwindow* window);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void processInput(GLFWwindow* window);
}



