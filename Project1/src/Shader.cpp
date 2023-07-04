#include "../include/Shader.h"

Shader::Shader()
{
}

Shader::Shader(const char* vertexName, const char* fragmentName)
{
    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::vector<char*> shaderPaths = getShaderPath(vertexName, fragmentName);
    char* vertexShaderPath = shaderPaths.at(0);
    char* fragmentShaderPath = shaderPaths.at(1);

    try
    {
        // 打开文件
        vShaderFile.open(vertexShaderPath);
        fShaderFile.open(fragmentShaderPath);
        std::stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到数据流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // 关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();
        // 转换数据流到string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << "——" << e.code() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    delete[] vertexShaderPath;
    delete[] fragmentShaderPath;

    // 2. 编译着色器
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // 顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // 打印编译错误
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // 片段着色器
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // 打印编译错误
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // 着色器程序
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // 打印链接错误
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 删除着色器
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setVector2(const std::string& name, glm::vec2 value) const
{
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y);
}

void Shader::setVector3(const std::string& name, glm::vec3 value) const
{
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVector4(const std::string& name, glm::vec4 value) const
{
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

std::vector<char*> Shader::getShaderPath(const char* vertexName, const char* fragmentName)
{
	std::vector<char*> shaderPaths;

	// Vertex Shader
	std::string vertexShaderPath = std::string(shaderPath) + "/" + std::string(VertexPath) + "/" + std::string(vertexName) + ".vs";
	char* vertexShaderPathChar = new char[vertexShaderPath.length() + 1];
    strcpy_s(vertexShaderPathChar, vertexShaderPath.length() + 1, vertexShaderPath.c_str());
	shaderPaths.push_back(vertexShaderPathChar);

	// Fragment Shader
	std::string fragmentShaderPath = std::string(shaderPath) + "/" + std::string(FragmentPath) + "/" + std::string(fragmentName) + ".fs";
	char* fragmentShaderPathChar = new char[fragmentShaderPath.length() + 1];
    strcpy_s(fragmentShaderPathChar, fragmentShaderPath.length() + 1, fragmentShaderPath.c_str());
	shaderPaths.push_back(fragmentShaderPathChar);

	return shaderPaths;
}

std::vector<char*> Shader::getShaderPath(const char* shaderName)
{
	return getShaderPath(shaderName, shaderName);
}
