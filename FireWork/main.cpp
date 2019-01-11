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
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//Զ���ü���
const float CAM_NEAR = 1.0f;
const float CAM_FAR = 20.0f;
// ���ڴ�С����
const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 720;
//��ʼ�����
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
//��ʱ��ʼ��
float deltaTime = 0.0f;
float lastTime = 0.0f;
// ��ͣ
bool isStop = false;
// ��¼��ͼ��������
int max_example = 100;
int count_rgb = 35846;
int count_depth =35846;


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
	// ����Ƿ����
	if (isMouse) glfwSetCursorPosCallback(window, mouse_callback);
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
	Shader ourShader("shader/particle_optimization.vs", "shader/particle_optimization.fs");
	// load models
	Model ourModel("obj/ball/3.obj");
	Operate operate;
	vector<Particle> particles;
	//operate.setParameter();//��ʼʹ��Ĭ�ϵĲ���
	operate.initParticles(particles, ourModel, ourShader);//��ʼ������
	operate.bu_particles = particles;//�����̻�����
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// ����ʱ��
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
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, CAM_NEAR, CAM_FAR);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		//��shader�д���Զ���ü���
		ourShader.setFloat("near", CAM_NEAR);
		ourShader.setFloat("far", CAM_FAR);
		//�����̻�������
		ourShader.setVec3("mColor", operate.parameter.color);//д�����ӵ���ɫ
		ourShader.setInt("saveNum", operate.m_saveTailNum);//�������ӵĸ���
		ourShader.setInt("interNum", operate.parameter.interTailNum);//�м��ֵ�ĸ���
		ourShader.setBool("isTailScale", operate.parameter.isTailScale);//β�������Ƿ�����
		//��ǰ��Ⱦ���Ƿ�ΪRGBͼ��
		ourShader.setBool("isRGB", operate.isRGB);
		for (int i = 0; i < particles.size(); i++) {
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f, 1.8f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glm::mat4 modelMat = model;
			// ������ӻ�û���������Ż���
			if (operate.isDie(particles[i], isStop)) {
				if (!isStop) operate.motion(particles[i]);//�����˶�	
				operate.renderOptimization(particles[i], modelMat);// �����Ż�
			}
		}
		// ��Ⱦ����ǰ֡
		glfwSwapBuffers(window);
		// ���浱ǰ֡
		if (isScreenshot && !isStop) {
			if(operate.isRGB)
				saveImage(operate.isRGB, operate.example, count_rgb, SCR_WIDTH, SCR_HEIGHT);
			else
				saveImage(operate.isRGB, operate.example, count_depth, SCR_WIDTH, SCR_HEIGHT);
		} 
		// ��ͣ
		if (!isStop) {
			operate.countFrame++;
			if(operate.isRGB)
				count_rgb++;
			else 
				count_depth++;
		}
		// ������������ʧʱ�����ò���
		if (operate.countFrame > operate.maxLife) {
			// ��ǰ�ǵڼ���example
			cout << "example:" << operate.example << endl;
			if (operate.example == max_example) {
				isStop = !isStop; // ����ʵ������ֹͣ��ͼ
			} 
			//cout << "��ը�ķ�Χ:" << getExplosionRangle(particles) << endl; //�������ը���ĵķ�Χ	
			operate.resetCamera(camera);
			operate.resetParticle(particles, ourModel, ourShader);
			operate.isRGB = !operate.isRGB;
		}
		
		
		//glfw: swap and poll
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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
	// �ո���ͣ
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		isStop = !isStop;
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




