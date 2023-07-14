#include "../include/display.h"
#include "../include/utils.h"
#include "../include/Shader.h"
#include "../include/Plane.h"
#include "../include/Box.h"
#include "../include/Mesh.h"
#include "../include/Model.h"
#include "../include/Camera.h"
#include "../include/Sphere.h"
#include "../include/PhysSphere.h"
#include "../include/Light.h"
#include "../include/PhysModel.h"
#include "../include/CylinderBox.h"

#include <glm/gtc/random.hpp>

using namespace display;

// 分辨率设置
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// 摄像机设置
Camera camera(glm::vec3(0.0f, 0.0f, 1.8f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

// 按键设置
bool First_R_Key = true;
bool First_S_Key = true;

// 物体位置参数
    // ------------
vector<glm::vec3> tumblerPosition = { glm::vec3(0.3f, -0.45f, 0.2f), glm::vec3(-0.3f, -0.45f, -0.3f), glm::vec3(0.0f, -0.45f, 0.0f) };
vector<glm::vec3> spherePosition = {
    glm::vec3(0.0907949f, -0.399212f, -0.208774f), glm::vec3(-0.0186592f, -0.202571f, -0.213606f), glm::vec3(-0.0883394f, -0.287907f, -0.383346f),
    glm::vec3(-0.273077f, 0.264156f, -0.181063f), glm::vec3(0.0192792f, 0.310677f, 0.360827f), glm::vec3(-0.252072f, 0.309068f, 0.0559487f),
    glm::vec3(-0.302903f, -0.0365484f, 0.308075f), glm::vec3(-0.198631f, -0.303337f, 0.315939f), glm::vec3(-0.347168f, -0.209269f, -0.343179f),
    glm::vec3(-0.353572f, -0.262772f, 0.230868f),
};

// 资产
vector<PhysSphere*> spheres;
vector<PhysModel*> tumblers;

// 渲染显示
void Display(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(5.0f);

    // 创建着色器
    // ----------
    Shader boxShader("box", "box");
    Shader modelShader("model", "model");
    Shader sphereShader("sphere", "sphere");
    Shader lineShader("line", "line");

    // 创建光源
    Light pointLight(LIGHT_TYPE::POINT);
    pointLight.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    // 创建box对象
    // -----------
    Box box;
    box.setShader(boxShader);
    box.setData();

    // 创建球体对象
    // ------------
    for (int i = 0; i < 10; i++) {
        PhysSphere* sphere = new PhysSphere(spherePosition.at(i), 0.01f, 30, 30, 1.0f);
        sphere->setAcc(glm::vec3(0.0f, -0.0f, 0.0f));
        sphere->setColor(glm::vec3(1.0f));
        sphere->setShader(sphereShader);
        spheres.push_back(sphere);
    }
    
    // 读取模型
    // --------
    std::string modelPath = "Resource/Model/tumbler/tumbler.obj";
    for (int i = 0; i < 3; i++) {
        PhysModel* tumbler = new PhysModel(tumblerPosition.at(i), 1.0f, const_cast<char*>(modelPath.c_str()));
        tumbler->setFric(0.01f);
        tumblers.push_back(tumbler);
    }

    // 渲染循环
    // --------
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        // ------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        // 渲染
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 着色器设置
        // ----------
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        vector<glm::mat4> tumblerModel(tumblers.size(), glm::mat4(1.0f));
        vector<glm::mat4> sphereModel(spheres.size(), glm::mat4(1.0f));
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(2, 2, 2));
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        box.setMatrix(model, view, projection);

        // 绘制场景
        // --------
        boxShader.use();
        pointLight.apply(boxShader, camera);
        box.draw();
        // 绘制小球
        // --------
        sphereShader.use();
        pointLight.apply(sphereShader, camera);
        for (int i = 0; i < spheres.size(); i++) {
            PhysSphere* sphere = spheres.at(i);
            glm::mat4 _model = sphere->update(deltaTime);
            sphere->setMatrix(_model, view, projection);
            sphere->aabb->update(_model);
            sphere->draw();
        }
        // 绘制不倒翁
        // ----------
        modelShader.use();
        pointLight.apply(modelShader, camera);
        for (int i = 0; i < tumblers.size(); i++) {
            // 位姿更新
            glm::mat4 _model = tumblers.at(i)->update(deltaTime);
            tumblerModel.at(i) = _model;
            modelShader.setMatrix4("model", _model);
            modelShader.setMatrix4("view", view);
            modelShader.setMatrix4("projection", projection);
            tumblers.at(i)->Draw(modelShader);

            // 计算碰撞
            CylinderBox* cylinder = tumblers.at(i)->cylinder;
            cylinder->update(_model);
            for (int j = 0; j < spheres.size(); j++) {
                glm::vec3 hit_normal;
                PhysSphere* sphere = spheres.at(j);
                bool is_interact = cylinder->intersect(sphere, hit_normal);

                if (is_interact) {
                    spheres.at(j)->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
                    glm::vec3 velocitySphere = spheres.at(j)->getVel();
                    spheres.at(j)->setVel(velocitySphere - 2.0f * glm::dot(hit_normal, velocitySphere) * hit_normal);
                }
            }
            /*AABB* aabb = tumblers.at(i)->aabb;
            
            aabb->update(_model);
            for (int j = 0; j < spheres.size(); j++) {
                bool is_interact = aabb->intersect(spheres.at(j)->aabb);
                if (is_interact) {
                    spheres.at(j)->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
                    glm::vec3 boundNormal = glm::normalize(spheres.at(j)->getPos() - tumblers.at(i)->getPos());
                    glm::vec3 velocitySphere = spheres.at(j)->getVel();
                    spheres.at(j)->setVel(velocitySphere - 2.0f * glm::dot(boundNormal, velocitySphere) * boundNormal);
                }
            }*/
        }

        // aabb可视化
        lineShader.use();
        for (int i = 0; i < tumblers.size(); i++) {
            CylinderBox* cylinder = tumblers.at(i)->cylinder;

            glm::vec3* axis = cylinder->getPhysAxis();
            glm::vec3 min = axis[0];
            glm::vec3 max = axis[1];

            lineShader.setMatrix4("model", glm::mat4(1.0f));
            lineShader.setMatrix4("view", view);
            lineShader.setMatrix4("projection", projection);

            vector<glm::vec3> pts = { min, max };

            lineSegment(min, max, glm::vec3(1.0f, 1.0f, 0.0f), lineShader);
            pointsDraw(pts, glm::vec3(1.0f, 1.0f, 0.0f), lineShader);
        }
        /*lineShader.use();
        for (int i = 0; i < spheres.size(); i++) {
            AABB* aabb = spheres.at(i)->aabb;
            
            glm::vec3 min = aabb->getMin();
            glm::vec3 max = aabb->getMax();

            glm::mat4 _model = aabb->update(spheres.at(i)->getMat(MATRIX_TYPE::MODEL));

            lineShader.setMatrix4("model", _model);
            lineShader.setMatrix4("view", view);
            lineShader.setMatrix4("projection", projection);

            vector<glm::vec3> pts = { min, max };

            lineSegment(min, max, glm::vec3(1.0f, 0.0f, 1.0f), lineShader);
            pointsDraw(pts, glm::vec3(1.0f, 0.0f, 1.0f), lineShader);
        }*/

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (int i = 0; i < tumblers.size(); i++) {
        delete tumblers.at(i);
    }
    for (int i = 0; i < spheres.size(); i++) {
        delete spheres.at(i);
    }
    tumblers.clear();
    spheres.clear();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void display::processInput(GLFWwindow* window)
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
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (First_R_Key) {
            First_R_Key = false;
            InitSphere();

            for (int i = 0; i < tumblers.size(); i++) {
                PhysModel* tumbler = tumblers.at(i);
                //tumbler->setPosAngle(glm::vec3(50.0f, 0.0f, 50.0f));
                //tumbler->setVelAngle(glm::vec3(50.0f, 200.0f, 50.0f));
                //tumbler->setVel(glm::vec3(0.5f, 0.0f, 0.0f));
                tumbler->setAcc(glm::vec3(0.0f, 0.0f, 0.0f));
            }
        }
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if (First_S_Key) {
            First_R_Key = false;
            First_S_Key = false;

            for (int i = 0; i < spheres.size(); i++) {
                PhysSphere* sphere = spheres.at(i);
                sphere->stop = true;
                sphere->setAcc(glm::vec3(0.0f, -0.98f, 0.0f));
                sphere->setVel(glm::vec3(0.0f));
            }

            for (int i = 0; i < tumblers.size(); i++) {
                PhysModel* tumbler = tumblers.at(i);
                tumbler->stop = true;
                tumbler->setAcc(glm::vec3(0.0f));
                tumbler->setVel(glm::vec3(0.0f));
                tumbler->setPos(tumblerPosition.at(i));
            }
        }
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        First_R_Key = true;
        First_S_Key = true;

        for (int i = 0; i < spheres.size(); i++) {
            PhysSphere* sphere = spheres.at(i);
            sphere->stop = false;
            sphere->setAcc(glm::vec3(0.0f));
            sphere->setVel(glm::vec3(0.0f));
            sphere->setPos(spherePosition.at(i));
            sphere->setColor(glm::vec3(1.0f));
            sphere->setAlpha(1.0f);
        }

        for (int i = 0; i < tumblers.size(); i++) {
            PhysModel* tumbler = tumblers.at(i);
            tumbler->stop = false;
        }
    }
        
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void display::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void display::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void InitSphere()
{
    for (int i = 0; i < spheres.size(); i++) {
        glm::vec3 velocity = glm::ballRand(1.0f) * glm::linearRand(0.5f, 1.5f);
        spheres.at(i)->setVel(velocity);
    }
}
