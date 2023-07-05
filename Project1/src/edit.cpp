#include "../include/edit.h"
#include "../include/utils.h"
#include "../include/Shader.h"
#include "../include/Plane.h"
#include "../include/Box.h"
#include "../include/Mesh.h"
#include "../include/Model.h"
#include "../include/Camera.h"

using namespace edit;

// 分辨率设置
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// 辅助线
vector<glm::vec3> xAxis = { glm::vec3(-1.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0) };
vector<glm::vec3> yAxis = { glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 1.0, 0.0) };
vector<glm::vec3> axisColor = { glm::vec3{0.5, 0.5, 0.0}, glm::vec3(0.0, 0.5, 0.5) };

void edit::Edit(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorPosCallback(window, mouse_callback);

    // 创建着色器
    // ----------
    Shader lineShader("line", "line");

    // 渲染循环
    // --------
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        // ------
        processInput(window);

        // 渲染
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 创建参考辅助线
        lineSegment(xAxis.at(0), xAxis.at(1), axisColor.at(0), lineShader);
        lineSegment(yAxis.at(0), yAxis.at(1), axisColor.at(1), lineShader);

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

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void edit::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);



}






