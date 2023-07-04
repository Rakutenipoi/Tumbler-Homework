#pragma once
#include <glad/glad.h> // ����glad����ȡ���еı���OpenGLͷ�ļ�
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Shader
{
public:
	// ��ɫ��ID
	unsigned int ID;
	
	Shader();

	Shader(const char* vertexName, const char* fragmentName);
	void use();

	// ��ɫ��Uniform�������ú���
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVector2(const std::string& name, glm::vec2 value) const;
	void setVector3(const std::string& name, glm::vec3 value) const;
	void setVector4(const std::string& name, glm::vec4 value) const;
	void setMatrix4(const std::string& name, glm::mat4 value) const;

private:
	const char* shaderPath = "Shader";
	const char* FragmentPath = "Fragment";
	const char* VertexPath = "Vertex";

	std::vector<char*> getShaderPath(const char* vertexName, const char* fragmentName);
	std::vector<char*> getShaderPath(const char* shaderName);
};

