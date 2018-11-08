#pragma once
#include <ctime>
#include <cmath>
#include <set>
#include "Particle.h"

class Operate {
public:
	int particleNumber = 100;//��������
	int totalTailNum;//����β���ܵ�������;
	int maxLife = 0;//��ȡ�������
	int countFrame = 0;//��¼֡��
	Parameter parameter;

	//�����̻�ϵͳ����
	void setParameter();
	//��ʼ����������
	void initParticles(vector<Particle>& particles, Model& ourModel, Shader& ourShader);
	//���ӵ��˶�ģ��
	void motion(Particle &particle);
	//���ӵ��������������ڼ��١��ߴ��С��͸���ȼ�С�������Ƿ����
	bool isDie(Particle &particle, bool isStop);
	//��ȡ���ӵ�β��λ��
	void getTailPosition(Particle &particle);
	//����β����ͷ��Ҳһ����ƣ�
	void renderTail(Particle &particle, glm::mat4 modelMat);
	//render �Ż�
	void renderOptimization(Particle &particle, glm::mat4 modelMat);

private:
	//���ɾ��ȷֲ��ķ���,������ɵ�����������particleNumber������ͬ
	vector<glm::vec3> genAverDir(int number);
	//������ȫ������Ҳ��ظ��ķ���
	vector<glm::vec3> genRandDir(int number);
	//���ݵ�ǰͷ���ӵ�size���ܹ�β��������total��ȡ��k������scale
	glm::vec3 getTailScale(float size, int k, int total);
	//���ݵ�ǰͷ���ӵ�transparent���ܹ�β����������ȡ��k�����ӵ�transparent
	float getTailTransparent(float transparent, int k, int total);
	//��ȡ[-1, 1]֮��������
	float getUnitRand();
}; 

///������ʵ��
//�����̻�ϵͳ����
void  Operate::setParameter() {
	// ����ϵͳ�����󣬿ɽ���һЩ�޸�
	// ��ȡ���Ƚϴ����ɫ���
	while (true) {
		float R = getUnitRand() / 2.0 + 0.5;
		float G = getUnitRand() / 2.0 + 0.5;
		float B = getUnitRand() / 2.0 + 0.5;
		float Y = 0.299*R + 0.587*G + 0.114*B;//��������
		if (Y >= 0.5) {
			parameter.color = glm::vec3(R, G, B);
			break;//����������������
		}
	}
}

//��ʼ����������
void Operate::initParticles(vector<Particle>& particles, Model& ourModel, Shader& ourShader) {
	particles.clear();
	srand(time(NULL));
	vector<glm::vec3> dirs = genAverDir(parameter.number);
	//vector<glm::vec3> dirs = genRandDir(particleNumber);
	particleNumber = dirs.size();//�����ܹ���������
	maxLife = parameter.initialLife + parameter.lifeBlur * parameter.initialLife;//��ȡ�������������ֵ
	totalTailNum = (parameter.saveTailNum - 1) * (parameter.interTailNum + 1);//������β��������
	float m_speed = parameter.initialSpeed;
	glm::vec3 m_accelerate = parameter.accelerate;
	int m_life = parameter.initialLife;
	float m_size = parameter.size;
	float m_transparent = parameter.transparent;
	for (int i = 0; i < dirs.size(); i++) {
		//����һ������
		Particle particle(i); //ID
		particle.particleModel = ourModel; //���ӵ�ģ��
		particle.particleShader = ourShader;//�������ӵ�shader
		particle.dir = dirs[i]; //���ӵĳ�ʼ����
		particle.speed = dirs[i] * (m_speed + m_speed * parameter.speedBlur * getUnitRand()); //���ӵĳ�ʼ�ٶ�,�����Ŷ�
		if (parameter.isResist) particle.accelerate = m_accelerate;//�����������,�������ӵļ��ٶ�
		particle.life = m_life + m_life * parameter.lifeBlur * getUnitRand();//���ӵ�����
		particle.tailPosition =list<glm::vec3>(parameter.saveTailNum, particle.position); // �洢��β��λ��
		particle.size = m_size + m_size * parameter.sizeBlur * getUnitRand();//���ӵĳߴ�
		particle.transparent = m_transparent + m_transparent * parameter.transparentBlur * getUnitRand();//���ӵĳ�ʼ͸����
		particles.push_back(particle);//�������ɵ�����
	}
}

//���ӵ��˶�ģ��
void Operate::motion(Particle &particle) {
	float t = parameter.delta_T;
	//����x����ļ��ٶȡ��ٶȡ�λ��
	//TODO�����ٶȻ�û���£��������ٶȵ�ƽ��������
	particle.speed -= particle.accelerate * t;
	particle.position += particle.speed * t;
	//�˶�֮����Ҫ���±����β������λ��
	getTailPosition(particle);
}

//���ӵ��������������ڼ��١��ߴ��С��͸���ȼ�С�������Ƿ����
bool Operate::isDie(Particle &particle, bool isStop) {
	//��ͣ���ӵ����Բ��ı�ֱ�ӷ��ؽ��л���
	if (isStop) return true;
	// ��������
	if (particle.life > 0) {
		particle.life--;
	}
	else {
		particleNumber--;
		return false;
	}
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
	if (countFrame % parameter.intervalFrame == 0) {
		glm::vec3 pos = particle.position;
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
			glm::vec3 tailScale = getTailScale(particle.size, m, totalTailNum);
			tmp_modelMat = glm::scale(tmp_modelMat, tailScale);
			//��ȡβ����͸����
			float tailTransparent = getTailTransparent(particle.transparent, m, totalTailNum);
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

//render �Ż����˴�ֻ����Ԥ��ֵ��λ�ã���ֵ�ļ��������shader��ִ�н�������֡��
void Operate::renderOptimization(Particle &particle, glm::mat4 modelMat) {
	//���뵱��model����
	particle.particleShader.setMat4("model", modelMat);
	//���뵱ǰͷ���ӵ�size
	particle.particleShader.setFloat("size", particle.size);
	//����alpha
	particle.particleShader.setFloat("alpha", particle.transparent);
	//����Ԥ��ֵ��λ��
	int m = 0;
	for (list<glm::vec3>::iterator itPos = particle.tailPosition.begin(); itPos != particle.tailPosition.end(); itPos++) {
		stringstream ss;
		string index;
		ss << m;
		index = ss.str();
		particle.particleShader.setVec3(("tailPos[" + index + "]").c_str(), *itPos);
		m++;
	}

}

//���ɾ��ȷֲ��ķ���
vector<glm::vec3> Operate::genAverDir(int number) {
	//srand(time(NULL));
	vector<glm::vec3> dirs;
	//��������ϵ
	float thetaInterval = PI / number;
	for (int i = 0; i <= number; i++) {
		float theta = i * thetaInterval;
		int m = std::max(floor(number * sin(theta)), 1.0f);
		float fiInterval = 2 * PI / m;
		for (int j = 0; j < m; j++) {
			float fi = j * fiInterval;
			//��theta��fi����С����Ŷ�
			theta += theta * parameter.dirBlur * getUnitRand();
			fi += fi * parameter.dirBlur * getUnitRand();
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
	srand(time(NULL));
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

//��ȡ[-1, 1]֮��������
float Operate::getUnitRand() {
	return rand() % 10000 / 10000.0 * 2.0 - 1;
}

