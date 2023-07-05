#include "../include/edit.h"
#include "../include/utils.h"
#include "../include/Shader.h"
#include "../include/Plane.h"
#include "../include/Box.h"
#include "../include/Mesh.h"
#include "../include/Model.h"
#include "../include/Camera.h"

using namespace edit;

// �ֱ�������
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1200;
bool click = false;

// ������
vector<glm::vec3> xAxis = { glm::vec3(-1.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0) };
vector<glm::vec3> yAxis = { glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 1.0, 0.0) };
vector<glm::vec3> axisColor = { glm::vec3{0.5, 0.5, 0.0}, glm::vec3(0.0, 0.5, 0.5) };

// ���Ƶ�����
vector<glm::vec3> controlPoints;
int index = 0;

glm::vec3 color(1.0f, 0.0f, 0.0f); // ������ɫΪ��ɫ

void Edit(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // ���õ������
    glPointSize(5.0f);

    // ������ɫ��
    // ----------
    Shader lineShader("line", "line");
    lineShader.use();

    GLuint vbo;
    glGenBuffers(1, &vbo);

    // ��Ⱦѭ��
    // --------
    while (!glfwWindowShouldClose(window))
    {
        // ����
        // ------
        processInput(window);

        // ��Ⱦ
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pointsDraw(controlPoints, lineShader);

        // �����ο�������
        lineSegment(xAxis.at(0), xAxis.at(1), axisColor.at(0), lineShader);
        lineSegment(yAxis.at(0), yAxis.at(1), axisColor.at(1), lineShader);

        // ������ɫ��uniform����
        float time = glfwGetTime();
        float t = (sin(time) + 1.0f) / 2.0f; // ��0��1֮��仯��tֵ

        /*if (index >= 3) {
            for (float t = 0.0f; t <= 1.0f; t += 0.01f) {
                glm::vec2 point = calculateBezierPoint(t);
                glBufferData(GL_ARRAY_BUFFER, sizeof(point), &point, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_POINTS, 0, 1);
            }
        }*/
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void edit::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void edit::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        float x = 2 * (float)xPos / SCR_WIDTH - 1.0f;
        float y = -(2 * (float)yPos / SCR_HEIGHT - 1.0f);

        if (x < 0)
            x = 0;

        controlPoints.push_back(glm::vec3(x, y, 0.0f));
        index++;

        printf("%f, %f\n", controlPoints.at(index - 1).x, controlPoints.at(index - 1).y);
    }
}

// ���㱴���������ϵĵ�
glm::vec2 calculateBezierPoint(float t)
{
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;

    glm::vec2 point;
    point.x = uu * controlPoints.at(index - 3).x + 2 * u * t * controlPoints.at(index - 2).x + tt * controlPoints.at(index - 1).x;
    point.y = uu * controlPoints.at(index - 3).y + 2 * u * t * controlPoints.at(index - 2).y + tt * controlPoints.at(index - 1).y;

    return point;
}




