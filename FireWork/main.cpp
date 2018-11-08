#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <list>

#include "shader.h"
#include "model.h"
#include "camera.h"
#include "Particle.h"
#include "Operate.h"
#include "Utils.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// 窗口大小设置
const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 720;
//初始化相机
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
//计时初始化
float deltaTime = 0.0f;
float lastTime = 0.0f;
// 暂停
bool isStop = false;

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FireWork", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// build and compile shaders
	//Shader ourShader("shader/particle.vs", "shader/particle.fs");
	Shader ourShader("shader/particle_optimization.vs", "shader/particle_optimization.fs");
	// load models
	Model ourModel("dataset/ball/3.obj");
	Operate operate;
	vector<Particle> particles;
	//operate.setParameter();//开始使用默认的参数
	operate.initParticles(particles, ourModel, ourShader);//初始化粒子
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// 更新时间
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		// input
		processInput(window);
		// rendering
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// enable shader
		ourShader.use();
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 50.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		//绘制烟花的粒子
		ourShader.setVec3("mColor", operate.parameter.color);//写入粒子的颜色
		ourShader.setInt("saveNum", operate.parameter.saveTailNum);//保存粒子的个数
		ourShader.setInt("interNum", operate.parameter.interTailNum);//中间插值的个数
		ourShader.setBool("isTailScale", operate.parameter.isTailScale);//尾部粒子是否缩放
		for (int i = 0; i < particles.size(); i++) {
			glm::mat4 model;
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glm::mat4 modelMat = model;
			// 如果粒子还没有消亡，才绘制
			if (operate.isDie(particles[i], isStop)) {
				if (!isStop) operate.motion(particles[i]);//粒子运动	
				//operate.renderTail(particles[i], modelMat);//为了绘制粒子而传递一些数据
				operate.renderOptimization(particles[i], modelMat);// 绘制优化
				particles[i].particleModel.Draw(ourShader, operate.totalTailNum);//绘制
				//ourShader.setMat4("model", model);
				//ourModel.Draw(ourShader);
			}
		}
		cout << "粒子总数:" << operate.particleNumber << endl;
		cout << "帧率：" << 1 / deltaTime << endl;
		// 所有生命都消失时，重置参数
		if (operate.countFrame > operate.maxLife) {
			operate.countFrame = 0;
			float range = 0;
			for (int i = 0; i < particles.size(); i++) {
				if (getDistance(particles[i].position) > range) {
					range = getDistance(particles[i].position);
				}
			}
			cout << "爆炸的范围:" << range << endl;//输出到爆炸中心的范围	
			operate.setParameter();//设置烟花粒子的系统参数
			operate.initParticles(particles, ourModel, ourShader);//初始化粒子
		}
		//glfw: swap and poll
		glfwSwapBuffers(window);
		glfwPollEvents();
		if(!isStop) operate.countFrame++;
	}
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	// 空格暂停
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		isStop = !isStop;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
