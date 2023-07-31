#include "../include/utils.h"

void lineSegment(glm::vec3 start, glm::vec3 end, glm::vec3 color, Shader shader)
{
	std::vector<glm::vec3> vertices;
	vertices.push_back(start);
	vertices.push_back(end);

	// 创建顶点缓冲对象
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	// 创建顶点数组对象
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	shader.setVector3("color", color);
	shader.use();

	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, 2);

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void pointsDraw(std::vector<glm::vec3> pts, glm::vec3 color, Shader shader)
{
	// 创建顶点缓冲对象
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pts.size() * sizeof(glm::vec3), pts.data(), GL_STATIC_DRAW);

	// 创建顶点数组对象
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	shader.setVector3("color", color);
	shader.use();

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, pts.size());

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void triangleDraw(std::vector<glm::vec3> pts, glm::vec3 color, Shader shader)
{
	// 创建顶点缓冲对象
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pts.size() * sizeof(glm::vec3), pts.data(), GL_STATIC_DRAW);

	// 创建顶点数组对象
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	shader.setVector3("color", color);
	shader.use();

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, pts.size());

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

glm::mat4 TranslateTowardsDirection(glm::vec3& direction, const float distance)
{
	glm::vec3 normalizedDirection = glm::normalize(direction); // 将方向向量归一化

	// 计算平移向量
	glm::vec3 translation = normalizedDirection * distance;

	// 创建平移矩阵
	glm::mat4 translationMatrix = glm::mat4(1.0f); // 单位矩阵
	translationMatrix[3] = glm::vec4(translation, 1.0f); // 设置平移向量

	return translationMatrix;
}

void PrintMat4(const glm::mat4& matrix)
{
	const float* ptr = glm::value_ptr(matrix); // 获取矩阵数据的指针

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << ptr[i + j * 4] << " ";
		}
		std::cout << std::endl;
	}
}

float generateRandomNumber()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0, 1.0);

	return dis(gen);
}

std::vector<glm::vec3> vectorSplit(glm::vec3 direction, float alpha, int n)
{
	std::vector<glm::vec3> directions;

	float theta = std::acos(direction.x); // 计算原向量与x轴的夹角
	float angleIncrement = alpha; // 新向量之间的夹角增量

	for (int i = 0; i < n; i++) {
		float theta_i = theta + glm::radians(angleIncrement * i); // 计算每个新向量与x轴的夹角

		float x = std::cos(theta_i); // 计算新向量的x分量
		float y = std::sin(theta_i); // 计算新向量的y分量

		glm::vec3 direction_i(x, y, 0.0f); // 组合新向量的分量

		directions.push_back(direction_i); // 将新向量添加到向量列表中
	}

	return directions;
}

glm::vec3 generateRandomDirection(int n)
{
	float angle = static_cast<float>(std::rand() % (n + 1)); // 生成0到n之间的随机角度
	angle = glm::radians(angle); // 将角度转换为弧度

	float x = std::cos(angle); // 计算随机方向的x分量
	float y = std::sin(angle); // 计算随机方向的y分量

	glm::vec3 randomDirection(x, y, 0.0f); // 组合随机方向的分量

	return randomDirection;
}
