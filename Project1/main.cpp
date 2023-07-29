#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "include/display.h"
#include "include/edit.h"
#include "include/test.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// 设置
// ----
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 1200;

enum MODE
{
    DISPLAY,
    EDIT,
    TEST,
};

// 模式选择
MODE mode = DISPLAY;

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
    // 设置窗口的显示位置
    glfwSetWindowPos(window, 200, 100);

    // 加载glad
    // ---------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    switch (mode)
    {
    case DISPLAY:
        Display(window);
        break;
    case EDIT:
        Edit(window);
        break;
    case TEST:
        Test(window);
        break;
    default:
        break;
    }
    

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

