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
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void processInput(GLFWwindow* window);
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void RotatePoints(std::vector<glm::vec3> &pts, std::vector<glm::vec3> &draw);
}

void Edit(GLFWwindow* window);
glm::vec2 calculateBezierPoint(float t);

