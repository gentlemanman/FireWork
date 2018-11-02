#pragma once
#include <ctime>
#include <cmath>
#include <set>
#include "Particle.h"

class Operate {
public:
	int particleNumber = 100;
	Parameter parameter;

	//�����̻�ϵͳ����
	void setParameter();
	//��ʼ����������
	void initParticles(vector<Particle>& particles, Model& ourModel, Shader& ourShader);
	//���ӵ��˶�ģ��
	void motion(Particle &particle);
	//���ӵ��������������ڼ��١��ߴ��С��͸���ȼ�С�������Ƿ����
	bool isDie(Particle &particle);
	//��ȡ���ӵ�β��λ��
	void getTailPosition(Particle &particle);
	//����β����ͷ��Ҳһ����ƣ�
	void renderTail(Particle &particle, glm::mat4 modelMat);

private:
	//���ɾ��ȷֲ��ķ���,������ɵ�����������particleNumber������ͬ
	vector<glm::vec3> genAverDir(int number);
	//������ȫ������Ҳ��ظ��ķ���
	vector<glm::vec3> genRandDir(int number);
	//���ݵ�ǰͷ���ӵ�size���ܹ�β��������total��ȡ��k������scale
	glm::vec3 getTailScale(float size, int k, int total);
	//���ݵ�ǰͷ���ӵ�transparent���ܹ�β����������ȡ��k�����ӵ�transparent
	float getTailTransparent(float transparent, int k, int total);
}; 


///������ʵ��
//�����̻�ϵͳ����
void  Operate::setParameter() {}

//��ʼ����������
void Operate::initParticles(vector<Particle>& particles, Model& ourModel, Shader& ourShader) {
	vector<glm::vec3> dirs = genAverDir(parameter.number);
	//vector<glm::vec3> dirs = genRandDir(particleNumber);
	for (int i = 0; i < dirs.size(); i++) {
		//����һ������
		Particle particle(i); //ID
		particle.particleModel = ourModel; //���ӵ�ģ��
		particle.particleShader = ourShader;//�������ӵ�shader
		particle.dir = dirs[i]; //���ӵĳ�ʼ����
		particle.speed = dirs[i] * parameter.initialSpeed; //���ӵĳ�ʼ�ٶ�
		if (parameter.isResist) particle.accelerate = parameter.accelerate;//�����������,�������ӵļ��ٶ�
		particle.life = parameter.initialLife;//���ӵ�����
		particle.size = parameter.size;//���ӵĳߴ�
		particle.transparent = parameter.transparent;//���ӵĳ�ʼ͸����
		particles.push_back(particle);//�������ɵ�����
	}
}

//���ӵ��˶�ģ��
void Operate::motion(Particle &particle) {
	float t = parameter.delta_T;
	//����x����ļ��ٶȡ��ٶȡ�λ��
	//TODO�����ٶȻ�û���£��������ٶȵ�ƽ���ɷ���
	particle.speed -= particle.accelerate * t;
	particle.position += particle.speed * t;
}

//���ӵ��������������ڼ��١��ߴ��С��͸���ȼ�С�������Ƿ����
bool Operate::isDie(Particle &particle) {
	// ��������
	if (particle.life > 0) {
		particle.life--;
	}
	else return false;
	// �ߴ�
	if (particle.size > 0) {
		particle.size -= parameter.sizeAtten;
	}
	else return false;
	// ͸����
	if (particle.transparent > 0) {
		particle.transparent -= parameter.transparentAtten;
	}
	else return false;

	return true;
}

//��ȡ���ӵ�β��λ��
void Operate::getTailPosition(Particle &particle) {
	glm::vec3 pos = particle.position; //��ǰ���ӵ�λ��,����֮ǰN�����ӵ�λ��
	if (particle.tailPosition.size() < parameter.saveTailNum) {
		particle.tailPosition.push_front(pos);
	}
	else {
		particle.tailPosition.pop_back();
		particle.tailPosition.push_front(pos);
	}
}

//����β����ͷ��Ҳһ����ƣ�
void Operate::renderTail(Particle &particle, glm::mat4 modelMat) {
	vector<glm::vec3> tailPos;
	for (list<glm::vec3>::iterator itPos = particle.tailPosition.begin(); itPos != particle.tailPosition.end(); ++itPos) {
		tailPos.push_back(*itPos);
	}
	int totalNum = (parameter.saveTailNum - 1) * (parameter.interTailNum + 1);//�����ܵ�������
	int interNum = parameter.interTailNum;//���м��N��λ�ó���
	int m = 0;
	for (int i = 0; i < tailPos.size() - 1; i++) {
		glm::vec3 start = tailPos[i];
		glm::vec3 end = tailPos[i + 1];
		glm::vec3 interval = (end - start) / (float)(interNum + 1);//������
		for (int j = 0; j <= interNum; j++) {
			//����β�����ӵ�Pos����model����
			glm::mat4 tmp_modelMat = modelMat;
			glm::vec3 interPos = start + interval * (float)j;
			tmp_modelMat = glm::translate(tmp_modelMat, interPos);
			glm::vec3 tailScale = getTailScale(particle.size, m, totalNum);
			tmp_modelMat = glm::scale(tmp_modelMat, tailScale);
			//��ȡβ����͸����
			float tailTransparent = getTailTransparent(particle.transparent, m, totalNum);
			//�����ɵ�model���󴫵ݵ�shader
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

//���ɾ��ȷֲ��ķ���
vector<glm::vec3> Operate::genAverDir(int number) {
	vector<glm::vec3> dirs;
	//��������ϵ
	float thetaInterval = PI / number;
	for (int i = 0; i <= number; i++) {
		float theta = i * thetaInterval;
		int m = std::max(floor(number * sin(theta)), 1.0f);
		float fiInterval = 2 * PI / m;
		for (int j = 0; j < m; j++) {
			float fi = j * fiInterval;
			//�����������
			glm::vec3 dir = glm::vec3(sin(theta)*sin(fi), cos(theta), sin(theta)*cos(fi));
			dirs.push_back(dir);
		}
	}
	return dirs;
}

//������ȫ������Ҳ��ظ��ķ���
vector<glm::vec3> Operate::genRandDir(int number) {
	vector<glm::vec3> dirs;
	//���
	float thetaInterval = PI / number;
	float fiInterval = 2 * PI / number;
	//����α�����������
	srand(time(0));
	//���ɲ��ظ����������
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
		// ����ʧ��ʱ�������������
		if (!p.second) {
			i--;
			continue;
		}
		float theta = m * thetaInterval;
		float fi = n * fiInterval;
		//�����������
		glm::vec3 dir = glm::vec3(sin(theta)*sin(fi), cos(theta), sin(theta)*cos(fi));
		dirs.push_back(dir);
	}
	return dirs;
}
//���ݵ�ǰͷ���ӵ�size,�͵�k�����ӻ�ȡscale
glm::vec3 Operate::getTailScale(float size, int k, int total) {
	if (parameter.isTailScale) {
		float interSize = size / (float)total;
		float curTailSize = (float)(total - k) * interSize;
		return glm::vec3(curTailSize);
	}
	return glm::vec3(size);
}

//���ݵ�ǰͷ���ӵ�transparent���ܹ�β����������ȡ��k�����ӵ�transparent
float Operate::getTailTransparent(float transparent, int k, int total) {
	float interTransparent = transparent / (float)total;
	float curTransparent = (float)(total - k) * interTransparent;
	return curTransparent;
}