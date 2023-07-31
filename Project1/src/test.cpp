#include "../include/test.h"
#include "../include/Camera.h"
#include "../include/Particle.h"
#include "../include/Light.h"
#include "../include/StaticMesh.h"

#define DEFAULT_SLICES 30
#define DEFAULT_STACKS 30

// 分辨率设置
static const unsigned int SCR_WIDTH = 1800;
static const unsigned int SCR_HEIGHT = 1200;

// 摄像机设置
static Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

static float deltaTime = 0.0f; // 当前帧与上一帧的时间差
static float lastFrame = 0.0f; // 上一帧的时间

// FPS计算
static int frameCount = 0;
static float previousTime = 0;

static const int PARTICLES_PER_EMITTER = 10; // 每个粒子发射器释放的粒子数

void Test(GLFWwindow* window)
{
    vector<glm::vec3> spherePosition = {
        // ------------------------------------------------------------------------------------------------------------------------------------------------
        glm::vec3(0.0907949f, -0.399212f, -0.208774f), glm::vec3(-0.0186592f, -0.202571f, -0.213606f), glm::vec3(-0.0883394f, -0.287907f, -0.383346f),
        glm::vec3(-0.273077f, 0.264156f, -0.181063f), glm::vec3(0.0192792f, 0.310677f, 0.360827f), glm::vec3(-0.252072f, 0.309068f, 0.0559487f),
        glm::vec3(-0.302903f, -0.0365484f, 0.308075f), glm::vec3(-0.198631f, -0.303337f, 0.315939f), glm::vec3(-0.347168f, -0.209269f, -0.343179f),
        glm::vec3(-0.353572f, -0.262772f, 0.230868f),
    };

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 创建着色器
    // ----------
    Shader sphereShader("sphere", "sphere");

    // 创建光源
    Light pointLight(LIGHT_TYPE::POINT);
    pointLight.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    // 创建粒子
    StaticSphere* sphere = new StaticSphere(1.0f, DEFAULT_SLICES, DEFAULT_STACKS);
    sphere->initBuffer();

    MeshParticle particle = MeshParticle();
    particle.setMesh(sphere, MESH_TYPE::SPHERE);
    particle.setParamVector3(vec3(0.4f, 0.8f, 1.0f), ATTRIB_TYPE::COLOR);
    particle.setParamFloat(1.0f, ATTRIB_TYPE::ALPHA);
    particle.setRadius(0.0005f);

    float bound = 0.2f;
    ParticleSystem ps;
    ps.setBoundary(bound);
    ps.friction = 0.1f;

    ParticleEmitter pe;
    pe.velocityInitialValue = 0.1f;
    pe.velocityTolerance = 0.05f;

    pe.positionInitialValue = vec3(-0.8f, 0.0f, 0.0f);
    
    // 渲染循环
    // --------
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        // ------
        frameCount++;
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float dt = currentFrame - previousTime;
        if (dt >= 1.0f) {
            float fps = frameCount / dt;
            std::cout << "FPS: " << fps << std::endl;
            frameCount = 0;
            previousTime = currentFrame;
        }

        processInput(window);
    
        // 粒子生成
        pe.generate(PARTICLES_PER_EMITTER, particle, ps, vec3(1.0f, 0.0f, 0.0f));

        // 基本参数
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
    
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        sphereShader.use();
        sphereShader.setMatrix4("view", view);
        sphereShader.setMatrix4("projection", projection);
        pointLight.apply(sphereShader, camera);
        ps.render(sphereShader);
        ps.update(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete sphere;
}





void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Position += cameraSpeed * camera.Front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Position -= cameraSpeed * camera.Front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Position -= glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Position += glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.Position += cameraSpeed * camera.WorldUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.Position -= cameraSpeed * camera.WorldUp;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        // Emit
        ;
}
