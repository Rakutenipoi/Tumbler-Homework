#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "include/utils.h"
#include "include/Shader.h"
#include "include/Plane.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 设置
// ----
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 lineColor[] = { glm::vec3(0.5f, 0.5f, 0.0f) , glm::vec3(0.0f, 0.5f, 0.5f) };
float planeVertices[] = {
    // positions          // texture coords
     0.5f,  0.5f, 0.0f,   // top right
     0.5f, -0.5f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int planeIndices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
};

int main()
{
    // 初始化glfw
    // -----------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建window
    // -----------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tumbler", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 加载glad
    // ---------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // 创建着色器
    // ----------
    // Shader lineShader("line", "line");
    Shader planeShader("plane", "plane");

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // 创建平面对象
    // ------------
    Plane plane;
    int numVertices = sizeof(planeVertices) / sizeof(planeVertices[0]);
    int numIndices = sizeof(planeIndices) / sizeof(planeIndices[0]);
    plane.setShader(planeShader);
    plane.setData(planeVertices, planeIndices, numVertices, numIndices);
    plane.setMatrix(model, view, projection);
    


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
        
        // lineSegment(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), lineColor[0], lineShader);
        // lineSegment(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), lineColor[1], lineShader);
        plane.draw(glm::vec3(0.5f, 0.5f, 0.8f));

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}