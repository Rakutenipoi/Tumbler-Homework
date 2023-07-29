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
#include "../include/utils.h"

using namespace display;

// 分辨率设置
static const unsigned int SCR_WIDTH = 1800;
static const unsigned int SCR_HEIGHT = 1200;
static const GLuint SHADOW_WIDTH = 1024;
static const GLuint SHADOW_HEIGHT = 1024;

// 摄像机设置
static Camera camera(glm::vec3(0.0f, 0.0f, 1.8f));
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

static float deltaTime = 0.0f; // 当前帧与上一帧的时间差
static float lastFrame = 0.0f; // 上一帧的时间

// FPS计算
static int frameCount = 0;
static float previousTime = 0;

// 按键设置
bool First_R_Key = true;
bool First_S_Key = true;
bool Ignore_Bound = false;

// 物体位置参数
// ------------
float ground = -0.425f;
vector<glm::vec3> tumblerPosition = { glm::vec3(0.3f, ground, 0.3f), glm::vec3(-0.3f, ground, -0.3f), glm::vec3(0.0f, ground, 0.0f),
    glm::vec3(-0.3f, ground, 0.3f), glm::vec3(0.3f, ground, -0.3f)};
vector<glm::vec3> spherePosition = {
    // ------------------------------------------------------------------------------------------------------------------------------------------------
    glm::vec3(0.0907949f, -0.399212f, -0.208774f), glm::vec3(-0.0186592f, -0.202571f, -0.213606f), glm::vec3(-0.0883394f, -0.287907f, -0.383346f),
    glm::vec3(-0.273077f, 0.264156f, -0.181063f), glm::vec3(0.0192792f, 0.310677f, 0.360827f), glm::vec3(-0.252072f, 0.309068f, 0.0559487f),
    glm::vec3(-0.302903f, -0.0365484f, 0.308075f), glm::vec3(-0.198631f, -0.303337f, 0.315939f), glm::vec3(-0.347168f, -0.209269f, -0.343179f),
    glm::vec3(-0.353572f, -0.262772f, 0.230868f), 
    glm::vec3(0.0907949f, -0.399212f, -0.208774f), glm::vec3(-0.0186592f, -0.202571f, -0.213606f), glm::vec3(-0.0883394f, -0.287907f, -0.383346f),
    glm::vec3(-0.273077f, 0.264156f, -0.181063f), glm::vec3(0.0192792f, 0.310677f, 0.360827f), glm::vec3(-0.252072f, 0.309068f, 0.0559487f),
    glm::vec3(-0.302903f, -0.0365484f, 0.308075f), glm::vec3(-0.198631f, -0.303337f, 0.315939f), glm::vec3(-0.347168f, -0.209269f, -0.343179f),
    glm::vec3(-0.353572f, -0.262772f, 0.230868f),
    
};

// 资产
vector<PhysSphere*> spheres;
vector<PhysModel*> tumblers;

// 创建粒子系统
ParticleSystem ps;
ParticleEmitter pe;
StaticSphere* sphere = new StaticSphere(1.0f, 30, 30);
MeshParticle particle = MeshParticle();
static const float DEFAULT_RADIUS = 0.02f;
static const int DEFAULT_LIFESPAN = 30;


// 渲染显示
void Display(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(10.0f);

    // 创建着色器
    // ----------
    Shader boxShader("box", "box");
    Shader modelShader("model", "model");
    Shader sphereShader("sphere", "sphere");
    Shader lineShader("line", "line");
    Shader shadowShader("shadow", "shadow", "shadow");

    // 创建光源
    Light pointLight(LIGHT_TYPE::POINT);
    pointLight.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    // 创建box对象
    // -----------
    Box box;
    box.setShader(boxShader);
    box.setData();

    // 粒子系统设置
    sphere->initBuffer();
    particle.setMesh(sphere, MESH_TYPE::SPHERE);
    particle.setParamVector3(vec3(1.0f), ATTRIB_TYPE::COLOR);
    particle.setParamFloat(1.0f, ATTRIB_TYPE::ALPHA);
    particle.setRadius(DEFAULT_RADIUS);

    ps.setBoundary(vec2(-0.5f, 0.5f));

    pe.velocityInitialValue = 5e-3f;
    pe.velocityTolerance = 2e-3f;
    
    // 读取模型
    // --------
    std::string modelPath = "Resource/Model/tumbler/tumbler.obj";
    for (int i = 0; i < tumblerPosition.size(); i++) {
        PhysModel* tumbler = new PhysModel(tumblerPosition.at(i), 1.0f, const_cast<char*>(modelPath.c_str()));
        tumbler->setFric(0.01f);
        tumblers.push_back(tumbler);
    }

    // 阴影
    // ----------------------------------------------------------------------------------------------------
    // 创建帧缓冲对象
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    // 创建深度立方体贴图
    GLuint depthCubemap;
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

    for (GLuint i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }  

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // 将深度纹理设置为深度缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // ----------------------------------------------------------------------------------------------------

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

        // 基本参数
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        vector<glm::mat4> tumblerModel(tumblers.size(), glm::mat4(1.0f));
        vector<glm::mat4> sphereModel(spheres.size(), glm::mat4(1.0f));

        // 渲染
        // ------
        // 先渲染深度贴图
        // ----------------------------------------------------------------------------------------------------
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        vector<glm::mat4> shadowTransforms = configLightMatrix(pointLight);
        // 着色器设置
        shadowShader.use();
        for (int i = 0; i < 6; i++) {
            shadowShader.setMatrix4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms.at(i));
        }
        shadowShader.setFloat("far_plane", 100.0f);
        shadowShader.setVector3("lightPos", pointLight.position);
        box.draw();
        shadowShader.use();
        /*for (int i = 0; i < spheres.size(); i++) {
            PhysSphere* sphere = spheres.at(i);
            sphereModel.at(i) = sphere->update(deltaTime);
            shadowShader.setMatrix4("model", sphereModel.at(i));
            sphere->draw(shadowShader);
        }*/
        for (int i = 0; i < tumblers.size(); i++) {
            // 位姿更新
            glm::mat4 _model = tumblers.at(i)->update(deltaTime);
            tumblerModel.at(i) = _model;
            shadowShader.setMatrix4("model", _model);
            //modelShader.setFloat("far_plane", 100.0f);
            tumblers.at(i)->Draw(shadowShader);

            CylinderBox* cylinder = tumblers.at(i)->cylinder;
            cylinder->update(_model);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // ----------------------------------------------------------------------------------------------------
        // 常规渲染并使用深度贴图
        // ----------------------------------------------------------------------------------------------------
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 着色器设置
        // ----------
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(2, 2, 2));
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        box.setMatrix(model, view, projection);

        // 绘制场景
        // --------
        boxShader.use();
        pointLight.apply(boxShader, camera);
        boxShader.setFloat("far_plane", 100.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        box.draw();
        // 绘制小球
        // ------------------------------------------------------------------------------------------------
        //sphereShader.use();
        //pointLight.apply(sphereShader, camera);
        //for (int i = 0; i < spheres.size(); i++) {
        //    PhysSphere* sphere = spheres.at(i);
        //    // glm::mat4 _model = sphere->update(deltaTime);
        //    sphere->setMatrix(sphereModel.at(i), view, projection);
        //    sphere->draw();
        //}
        // 绘制粒子
        // --------------------------------------------------------------------------------------------------
        sphereShader.use();
        sphereShader.setMatrix4("view", view);
        sphereShader.setMatrix4("projection", projection);
        pointLight.apply(sphereShader, camera);
        // 粒子破碎效果
        vector<int> eraseList;
        int size = ps.getSize();
        for (int i = 0; i < size; i++) {
            vector<Particle*> particles = ps.getParticles();
            MeshParticle* target = dynamic_cast<MeshParticle*>(particles.at(i));

            bool is_dead = target->isDead();
            if (is_dead) {
                float radius = target->getRadius();
                
                // 避免粒子过小
                if (radius >= DEFAULT_RADIUS / 5) {
                    eraseList.push_back(i);
                    glm::vec3 direction = target->getParamVector3(ATTRIB_TYPE::DIRECTION);
                    float velocity = target->getParamFloat(ATTRIB_TYPE::VELOCITY);
                    std::vector<glm::vec3> directions = vectorSplit(direction, 30.0f);

                    vector<Particle*> newParticles;
                    for (int j = 0; j < 4; j++) {
                        MeshParticle* newParticle = new MeshParticle();
                        *newParticle = *target;

                        newParticle->setRadius(radius / 2);
                        newParticle->setParamFloat(velocity * 0.8, ATTRIB_TYPE::VELOCITY);
                        newParticle->setParamVector3(directions.at(j), ATTRIB_TYPE::DIRECTION);
                        newParticle->setParamVector3(target->getParamVector3(ATTRIB_TYPE::COLOR), ATTRIB_TYPE::COLOR);
                        newParticle->setParamVector3(target->getParamVector3(ATTRIB_TYPE::POSITION), ATTRIB_TYPE::POSITION);
                        newParticle->setParamFloat(target->getParamFloat(ATTRIB_TYPE::ALPHA), ATTRIB_TYPE::ALPHA);
                        newParticle->setParamInteger(DEFAULT_LIFESPAN, ATTRIB_TYPE::LIFESPAN);

                        newParticles.push_back(newParticle);
                    }

                    ps.add(newParticles);
                }
            }
        }
        for (int i : eraseList) {
            ps.erase(i);
        }
        // 粒子渲染与位姿更新
        ps.render(sphereShader);
        ps.update(deltaTime);
        // 绘制不倒翁
        // --------------------------------------------------------------------------------------------------
        modelShader.use();
        pointLight.apply(modelShader, camera);
        for (int i = 0; i < tumblers.size(); i++) {
            // 位姿更新
            //glm::mat4 _model = tumblers.at(i)->update(deltaTime);
            //tumblerModel.at(i) = _model;
            modelShader.setMatrix4("model", tumblerModel.at(i));
            modelShader.setMatrix4("view", view);
            modelShader.setMatrix4("projection", projection);
            //modelShader.setFloat("far_plane", 100.0f);
            tumblers.at(i)->Draw(modelShader);
   
            //CylinderBox* cylinder = tumblers.at(i)->cylinder;
            //cylinder->update(_model);
        }
        // ----------------------------------------------------------------------------------------------------

        // 计算碰撞
        if (!Ignore_Bound) {
            for (int i = 0; i < tumblers.size(); i++) {
                CylinderBox* cylinder = tumblers.at(i)->cylinder;

                for (Particle* particle : ps.getParticles()) {
                    MeshParticle* target = dynamic_cast<MeshParticle*>(particle);
                    glm::vec4 hit_normal;
                    bool is_interact = cylinder->intersect(*target, hit_normal);

                    if (is_interact) {
                        target->setParamInteger(target->getParamInteger(ATTRIB_TYPE::LIFESPAN) - 1, ATTRIB_TYPE::LIFESPAN);
                        glm::vec3 hitNormal = glm::vec3(hit_normal);

                        // 小球变色
                        target->setParamVector3(glm::vec3(1.0f, 0.2f, 0.3f), ATTRIB_TYPE::COLOR);

                        // 小球反弹
                        glm::vec3 direction = target->getParamVector3(ATTRIB_TYPE::DIRECTION);
                        direction += 2.0f * glm::max(glm::dot(-hitNormal, direction), 0.0f) * hitNormal;
                        target->setParamVector3(direction, ATTRIB_TYPE::DIRECTION);
                        
                        // 小球速度大小
                        float velocitySphereValue = target->getParamFloat(ATTRIB_TYPE::VELOCITY);

                        // 碰撞平移
                        glm::vec3 velocityTranslate = glm::vec3(hitNormal.x, 0.0f, hitNormal.z);
                        float velocityTranslateRate = 0.8f; // 速度调节比率
                        tumblers.at(i)->addValue(-velocityTranslate * velocityTranslateRate * velocitySphereValue, PHYS_PARAM_TYPE::VELOCITY);

                        // 碰撞旋转
                        float velocityRotateRate = 250.0f;
                        float volocityRotate = 1 - glm::dot(direction, -velocityTranslate);
                        tumblers.at(i)->addValue(glm::vec3(0.0f, volocityRotate * velocityRotateRate * velocitySphereValue, 0.0f), PHYS_PARAM_TYPE::ANGLE_VELOCITY);

                        // 碰撞摇摆
                        float velocitySwingRate = 1000.0f;
                        glm::vec3 velocitySwing = glm::vec3(-hitNormal.z, 0.0f, hitNormal.x) * hit_normal.w * velocitySwingRate * velocitySphereValue;
                        tumblers.at(i)->addValue(velocitySwing, PHYS_PARAM_TYPE::ANGLE_VELOCITY);
                    }
                }
                

                for (int j = 1; j < tumblerPosition.size(); j++) {
                    int idx = (i + j) % tumblerPosition.size();
                    glm::vec3 hitNormal;
                    bool is_interact = cylinder->intersect(tumblers.at(idx)->cylinder->getPhysAxis()[0], tumblers.at(idx)->cylinder->getPhysAxis()[1], tumblers.at(idx)->cylinder->getRadiusDown(), hitNormal);
                    float velocityBoundRate = 0.05f;

                    if (is_interact) {
                        // 碰撞反弹
                        glm::vec3 velocityTumbler = tumblers.at(i)->getVel();
                        tumblers.at(i)->addValue(-hitNormal * max(glm::dot(hitNormal, velocityTumbler) * 2 - hitNormal * velocityBoundRate, 0.0f), PHYS_PARAM_TYPE::VELOCITY);
                        velocityTumbler = tumblers.at(idx)->getVel();
                        tumblers.at(idx)->addValue(hitNormal * max(glm::dot(-hitNormal, velocityTumbler) * 2 + hitNormal * velocityBoundRate, 0.0f), PHYS_PARAM_TYPE::VELOCITY);
                    }
                }
            }
        }
        
        // aabb可视化
        /*lineShader.use();
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
        }*/
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
            //InitSphere();
            for (int i = 0; i < spherePosition.size(); i++) {
                pe.positionInitialValue = spherePosition.at(i);
                pe.generate(1, particle, ps);
            }

            for (int i = 0; i < tumblers.size(); i++) {
                PhysModel* tumbler = tumblers.at(i);
                //tumbler->setPosAngle(glm::vec3(0.0f, 0.0f, 0.0f));
                //tumbler->setVelAngle(glm::vec3(50.0f, 200.0f, 50.0f));
                //tumbler->setVel(glm::vec3(0.5f, 0.0f, 0.5f));
                tumbler->setAcc(glm::vec3(0.0f, 0.0f, 0.0f));
            }
        }
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        for (int i = 0; i < tumblers.size(); i++) {
            PhysModel* tumbler = tumblers.at(i);
            //tumbler->setPosAngle(glm::vec3(0.0f, 0.0f, 0.0f));
            tumbler->setVelAngle(glm::vec3(50.0f, 0.0f, 50.0f));
            tumbler->setVel(glm::vec3(0.5f, 0.0f, 0.5f));
            tumbler->setAcc(glm::vec3(0.0f, 0.0f, 0.0f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if (First_S_Key) {
            First_R_Key = false;
            First_S_Key = false;
            Ignore_Bound = true;

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
                tumbler->setPosAngle(glm::vec3(0.0f));
                tumbler->setVelAngle(glm::vec3(0.0f));
                tumbler->setAccAngle(glm::vec3(0.0f));
            }
        }
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        First_R_Key = true;
        First_S_Key = true;
        Ignore_Bound = false;

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
    std::srand(static_cast<unsigned int>(std::time(0)));
    
    for (int i = 0; i < spheres.size(); i++) {
        glm::vec3 velocity = glm::vec3(generateRandomNumber(), generateRandomNumber(), generateRandomNumber())
            * glm::linearRand(0.5f, 1.0f);
        spheres.at(i)->setVel(velocity);
    }
}

vector<glm::mat4> configLightMatrix(Light light)
{
    float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
    float near = 0.1f;
    float far = 100.0f;
    glm::mat4 shadowProjection = glm::perspective(glm::radians(90.0f), aspect, near, far);
    vector<glm::mat4> shadowTransforms;

    shadowTransforms.push_back(shadowProjection * glm::lookAt(light.position, light.position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProjection * glm::lookAt(light.position, light.position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProjection * glm::lookAt(light.position, light.position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    shadowTransforms.push_back(shadowProjection * glm::lookAt(light.position, light.position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
    shadowTransforms.push_back(shadowProjection * glm::lookAt(light.position, light.position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(shadowProjection * glm::lookAt(light.position, light.position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

    return shadowTransforms;
}
