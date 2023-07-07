#include "../include/utils.h"

void lineSegment(glm::vec3 start, glm::vec3 end, glm::vec3 color, Shader shader)
{
	std::vector<glm::vec3> vertices;
	vertices.push_back(start);
	vertices.push_back(end);

	// �������㻺�����
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	// ���������������
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
	// �������㻺�����
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pts.size() * sizeof(glm::vec3), pts.data(), GL_STATIC_DRAW);

	// ���������������
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
	// �������㻺�����
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pts.size() * sizeof(glm::vec3), pts.data(), GL_STATIC_DRAW);

	// ���������������
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
	glm::vec3 normalizedDirection = glm::normalize(direction); // ������������һ��

	// ����ƽ������
	glm::vec3 translation = normalizedDirection * distance;

	// ����ƽ�ƾ���
	glm::mat4 translationMatrix = glm::mat4(1.0f); // ��λ����
	translationMatrix[3] = glm::vec4(translation, 1.0f); // ����ƽ������

	return translationMatrix;
}

void PrintMat4(const glm::mat4& matrix)
{
	const float* ptr = glm::value_ptr(matrix); // ��ȡ�������ݵ�ָ��

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << ptr[i + j * 4] << " ";
		}
		std::cout << std::endl;
	}
}
