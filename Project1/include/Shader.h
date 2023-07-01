#pragma once
#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Shader
{
public:
	// 着色器ID
	unsigned int ID;
	
	Shader(const char* vertexName, const char* fragmentName);
	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	std::vector<char*> getShaderPath(const char* vertexName, const char* fragmentName);
	std::vector<char*> getShaderPath(const char* shaderName);

private:
	const char* shaderPath = "Shader";
	const char* FragmentPath = "Fragment";
	const char* VertexPath = "Vertex";
};

