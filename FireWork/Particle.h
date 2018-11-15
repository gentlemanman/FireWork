#pragma once
#define PI 3.1415927f
//是否截图
#define isScreenshot 0

//粒子系统的初始属性
#define NUMBER 10//粒子的数量
#define DELTA_T 0.02f //时间间隔
#define INITIALSPEED 2.0f //沿x，y，z方向的初始速度
#define ISRESIST 1;//是否添加阻力
#define AX 0.0f //x的加速度（阻力）
#define AY 0.0f //y的加速度（重力与阻力的合力）
#define AZ 0.0f //z的加速度（阻力）
#define INITIALLIFE 100 //生命周期
#define SAVETAILNUM 10 // 保存尾部粒子的个数
#define INTERTAILNUM 10 //在两个粒子之间插值的数量
#define SIZE 1.0f //粒子的初始Scale
#define SIZEATTEN 0.001f //粒子size衰减的速率(scale)
#define ISTAILSCALE 1 //尾部尺寸是否衰减
#define TRANSPARENT 1.0f //粒子的初始透明度
#define TRANSPARENTATTEN 0.001f //粒子透明度的衰减
#define INTERVALFRAME 2 //保存之前尾部位置的间隔帧数
#define SPEEDBLUR 0.02f //速度的扰动
#define DIRBLUR 0.02f //方向的扰动
#define LIFEBLUR 0.2 //生命的扰动
#define SIZEBLUR 0.02 //尺寸的扰动
#define TRANSPARENTBLUR 0.02 //透明度的扰动

struct Parameter {
	int number = NUMBER;
	float delta_T = DELTA_T;
	float initialSpeed = INITIALSPEED;
	bool isResist = ISRESIST;
	glm::vec3 accelerate = glm::vec3(AX, AY, AZ);
	int initialLife = INITIALLIFE;
	int saveTailNum = SAVETAILNUM;
	int interTailNum = INTERTAILNUM;
	float size = SIZE;
	float sizeAtten = SIZEATTEN;
	bool isTailScale = ISTAILSCALE;
	float transparent = TRANSPARENT;
	float transparentAtten = TRANSPARENTATTEN;
	int intervalFrame = INTERVALFRAME;
	float speedBlur = SPEEDBLUR;
	float dirBlur = DIRBLUR;
	float lifeBlur = LIFEBLUR;
	float sizeBlur = SIZEBLUR;
	float transparentBlur = TRANSPARENTBLUR;
	glm::vec3 color = glm::vec3(1.0f, 0.5f, 0.31f);
};
class Particle {
public:
	int ID;//粒子ID
	glm::vec3 dir;//粒子初始方向
	Model particleModel;//粒子模型
	Shader particleShader;//绘制当前粒子的shader
	glm::vec3 position;//粒子的当前位置
	glm::vec3 speed;//粒子沿不同方向的速度
	glm::vec3 accelerate;//粒子沿三个方向的加速度
	int life; //生命周期
	list<glm::vec3> tailPosition; //存储尾部的连续位置
	float size; //初始的scale
	float transparent; //粒子的初始透明度


public:
	//构造函数
	Particle() {}
	Particle(int id) {
		ID = id;
	}
	//析构函数
	~Particle() {}
	
};