#pragma once
#include <ctime>
#include <cmath>
#include <set>
#include "camera.h"
#include "Particle.h"
#include "Utils.h"

class Operate {
public:

	int particleNumber = 100;//总粒子数
	int totalTailNum;//计算尾部总的粒子数;
	int maxLife = 0;//获取最大生命
	int countFrame = 0;//记录当前烟花帧数
	int example = 0;//记录是第几个烟花
	bool isRGB = true;//当前的记录是RGB还是深度图
	vector<Particle> bu_particles;//备份上一次生成的所有粒子供生成深度图使用
	Parameter parameter;

	//设置烟花系统参数
	void setParameter();
	//初始化所有粒子
	void initParticles(vector<Particle>& particles, Model& ourModel, Shader& ourShader);
	//粒子的运动模型
	void motion(Particle &particle);
	//粒子的消亡，生命周期减少、尺寸减小、透明度减小，返回是否绘制
	bool isDie(Particle &particle, bool isStop);
	//获取粒子的尾部位置
	void getTailPosition(Particle &particle);
	//绘制尾部（头部也一块绘制）
	void renderTail(Particle &particle, glm::mat4 modelMat);
	//render 优化
	void renderOptimization(Particle &particle, glm::mat4 modelMat);
	// 有关重置烟花粒子
	void resetParticle(vector<Particle>& particles, Model& ourModel, Shader& ourShader);
	// 重置相机
	void resetCamera(Camera& camera);
private:
	//生成均匀分布的方向,最后生成的粒子数量与particleNumber并不相同
	vector<glm::vec3> genAverDir(int number);
	//生成完全随机的且不重复的方向
	vector<glm::vec3> genRandDir(int number);
	//根据当前头粒子的size、总共尾部粒子数total获取第k个粒子scale
	glm::vec3 getTailScale(float size, int k, int total);
	//根据当前头粒子的transparent、总共尾部粒子数获取第k个粒子的transparent
	float getTailTransparent(float transparent, int k, int total);
	//获取所有粒子的最大生命值
	int getMaxLife(vector<Particle>& particles);
}; 

///函数的实现
//设置烟花系统参数
void  Operate::setParameter() {
	// 重置系统参数后，可进行一些修改
	parameter.color = getRandColor();//获取较亮的颜色
}

//初始化所有粒子
void Operate::initParticles(vector<Particle>& particles, Model& ourModel, Shader& ourShader) {
	particles.clear();
	srand(time(NULL));
	//vector<glm::vec3> dirs = genAverDir(parameter.number);
	vector<glm::vec3> dirs = getSpherePoint(100);
	//vector<glm::vec3> dirs = genRandDir(100);
	particleNumber = dirs.size();//保存总共的粒子数
	totalTailNum = (parameter.saveTailNum - 1) * (parameter.interTailNum + 1);//计算总尾部粒子数
	float m_speed = parameter.initialSpeed;
	glm::vec3 m_accelerate = parameter.accelerate;
	int m_life = parameter.initialLife;
	float m_size = parameter.size;
	float m_transparent = parameter.transparent;
	for (int i = 0; i < dirs.size(); i++) {
		//生成一个粒子
		Particle particle(i); //ID
		particle.particleModel = ourModel; //粒子的模型
		particle.particleShader = ourShader;//绘制粒子的shader
		particle.dir = dirs[i]; //粒子的初始方向
		particle.speed = dirs[i] * (m_speed + m_speed * parameter.speedBlur * getUnitRand()); //粒子的初始速度,加入扰动
		if (parameter.isResist) particle.accelerate = m_accelerate;//如果存在阻力,设置粒子的加速度
		particle.life = m_life + m_life * parameter.lifeBlur * getUnitRand();//粒子的生命
		particle.tailPosition =list<glm::vec3>(parameter.saveTailNum, particle.position); // 存储的尾部位置
		particle.size = m_size + m_size * parameter.sizeBlur * getUnitRand();//粒子的尺寸
		particle.transparent = m_transparent + m_transparent * parameter.transparentBlur * getUnitRand();//粒子的初始透明度
		particles.push_back(particle);//保存生成的粒子
	}
	maxLife = getMaxLife(particles);//获取粒子生命的最大值
}

//粒子的运动模型
void Operate::motion(Particle &particle) {
	float t = parameter.delta_T;
	//更新x方向的加速度、速度、位置
	//TODO：加速度还没更新，可以与速度的平方成正比
	particle.speed -= particle.accelerate * t;
	particle.position += particle.speed * t;
	//运动之后需要更新保存的尾部粒子位置
	getTailPosition(particle);
}

//粒子的消亡，生命周期减少、尺寸减小、透明度减小，返回是否绘制
bool Operate::isDie(Particle &particle, bool isStop) {
	//暂停粒子的属性不改变直接返回进行绘制
	if (isStop) return true;
	// 生命周期
	if (particle.life > 0) {
		particle.life--;
	}
	else {
		particleNumber--;
		return false;
	}
	// 尺寸
	if (particle.size > 0) {
		particle.size -= parameter.sizeAtten;
	}
	else return false;
	// 透明度
	if (particle.transparent > 0) {
		particle.transparent -= parameter.transparentAtten;
	}
	else return false;
	return true;
}

//获取粒子的尾部位置
void Operate::getTailPosition(Particle &particle) {
	if (countFrame % parameter.intervalFrame == 0) {
		glm::vec3 pos = particle.position;
		particle.tailPosition.pop_back();
		particle.tailPosition.push_front(pos);
	}
}

//绘制尾部（头部也一块绘制）
void Operate::renderTail(Particle &particle, glm::mat4 modelMat) {
	vector<glm::vec3> tailPos;
	for (list<glm::vec3>::iterator itPos = particle.tailPosition.begin(); itPos != particle.tailPosition.end(); ++itPos) {
		tailPos.push_back(*itPos);
	}
	int interNum = parameter.interTailNum;//在中间插N个位置出来
	int m = 0;
	for (int i = 0; i < tailPos.size() - 1; i++) {
		glm::vec3 start = tailPos[i];
		glm::vec3 end = tailPos[i + 1];
		glm::vec3 interval = (end - start) / (float)(interNum + 1);//计算间隔
		for (int j = 0; j <= interNum; j++) {
			//根据尾部粒子的Pos设置model矩阵
			glm::mat4 tmp_modelMat = modelMat;
			glm::vec3 interPos = start + interval * (float)j;
			tmp_modelMat = glm::translate(tmp_modelMat, interPos);
			glm::vec3 tailScale = getTailScale(particle.size, m, totalTailNum);
			tmp_modelMat = glm::scale(tmp_modelMat, tailScale);
			//获取尾部的透明度
			float tailTransparent = getTailTransparent(particle.transparent, m, totalTailNum);
			//将生成的model矩阵传递到shader
			stringstream ss;
			string index;
			ss << m;
			index = ss.str();
			particle.particleShader.setMat4(("modelMat[" + index + "]").c_str(), tmp_modelMat);
			particle.particleShader.setFloat(("alpha[" + index + "]").c_str(), tailTransparent);		
			m++;
		}
	}
}

//render 优化：此处只传入预插值的位置，插值的计算操作在shader中执行将大幅提高帧率
void Operate::renderOptimization(Particle &particle, glm::mat4 modelMat) {
	//传入当的model矩阵
	particle.particleShader.setMat4("model", modelMat);
	//传入当前头粒子的size
	particle.particleShader.setFloat("size", particle.size);
	//传入alpha
	particle.particleShader.setFloat("alpha", particle.transparent);
	//传入预插值的位置
	int m = 0;
	for (list<glm::vec3>::iterator itPos = particle.tailPosition.begin(); itPos != particle.tailPosition.end(); itPos++) {
		stringstream ss;
		string index;
		ss << m;
		index = ss.str();
		particle.particleShader.setVec3(("tailPos[" + index + "]").c_str(), *itPos);
		m++;
	}
	//绘制
	particle.particleModel.Draw(particle.particleShader, totalTailNum);

}

void Operate::resetParticle(vector<Particle>& particles, Model& ourModel, Shader& ourShader) {
	countFrame = 0;
	if (!isRGB) {
		setParameter();//设置烟花粒子系统的参数
		initParticles(particles, ourModel, ourShader);//初始化粒子
		bu_particles = particles;//备份粒子
		example++;//进行下一个实例
	}
	else {
		particles = bu_particles;
	}
}

void Operate::resetCamera(Camera& camera) {
	// 当前不是RGB帧时，下一example是RGB，需要更新相机的位置
	if (!isRGB) {
		camera.Position = getSpherePoint(1)[0] * 10.0f;
		camera.Front = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - camera.Position);
		camera.Right = glm::normalize(glm::cross(camera.Front, camera.WorldUp));
		camera.Up = glm::normalize(glm::cross(camera.Right, camera.Front));
	}
}

//生成均匀分布的方向
vector<glm::vec3> Operate::genAverDir(int number) {
	//srand(time(NULL));
	float rangeTheta = PI;
	vector<glm::vec3> dirs;
	//球面坐标系
	float thetaInterval = rangeTheta / number;
	for (int i = 0; i <= number; i++) {
		float theta = i * thetaInterval;
		int m = std::max(floor(number * sin(theta)), 1.0f);
		float fiInterval = 2 * PI / m;
		for (int j = 0; j < m; j++) {
			float fi = j * fiInterval;
			//对theta和fi加入小随机扰动
			theta += theta * parameter.dirBlur * getUnitRand();
			fi += fi * parameter.dirBlur * getUnitRand();
			//生成随机方向
			glm::vec3 dir = glm::vec3(sin(theta)*sin(fi), cos(theta), sin(theta)*cos(fi));
			dirs.push_back(dir);
		}
	}
	return dirs;
}

//生成完全随机的且不重复的方向
vector<glm::vec3> Operate::genRandDir(int number) {
	vector<glm::vec3> dirs;
	//间隔
	float thetaInterval = PI / number;
	float fiInterval = 2 * PI / number;
	//生成伪随机数的种子
	srand(time(NULL));
	//生成不重复的随机数对
	set<vector<int>> s;
	pair<set<vector<int>>::iterator, bool> p;
	vector<int> v;
	for (int i = 0; i < number; i++) {
		int m = rand() % (number + 1);
		int n = rand() % (number + 1);
		v.clear();
		v.push_back(m);
		v.push_back(n);
		p = s.insert(v);
		// 插入失败时重新生成随机数
		if (!p.second) {
			i--;
			continue;
		}
		float theta = m * thetaInterval;
		float fi = n * fiInterval;
		//生成随机方向
		glm::vec3 dir = glm::vec3(sin(theta)*sin(fi), cos(theta), sin(theta)*cos(fi));
		dirs.push_back(dir);
	}
	return dirs;
}
//根据当前头粒子的size,和第k个粒子获取scale
glm::vec3 Operate::getTailScale(float size, int k, int total) {
	if (parameter.isTailScale) {
		float interSize = size / (float)total;
		float curTailSize = (float)(total - k) * interSize;
		return glm::vec3(curTailSize);
	}
	return glm::vec3(size);
}

//根据当前头粒子的transparent、总共尾部粒子数获取第k个粒子的transparent
float Operate::getTailTransparent(float transparent, int k, int total) {
	float interTransparent = transparent / (float)total;
	float curTransparent = (float)(total - k) * interTransparent;
	return curTransparent;
}

//获取所有粒子的最大生命值
int Operate::getMaxLife(vector<Particle>& particles) {
	int ans = 0;
	for (Particle particle : particles) {
		if (particle.life > ans) {
			ans = particle.life;
		}
	}
	return ans;
}