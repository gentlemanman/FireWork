#pragma once
#define PI 3.1415927f

//����ϵͳ�ĳ�ʼ����
#define NUMBER 10 //���ӵ�����
#define DELTA_T 0.03f //ʱ����
#define INITIALSPEED 2.0f //��x��y��z����ĳ�ʼ�ٶ�
#define ISRESIST 1;//�Ƿ���������
#define AX 0.0f //x�ļ��ٶȣ�������
#define AY 1.0f //y�ļ��ٶȣ������������ĺ�����
#define AZ 0.0f //z�ļ��ٶȣ�������
#define INITIALLIFE 1000 //��������
#define SAVETAILNUM 10 // ����β�����ӵĸ���
#define INTERTAILNUM 4 //����������֮���ֵ������
#define SIZE 1.0f //���ӵĳ�ʼScale
#define SIZEATTEN 0.005f //����size˥��������(scale)
#define TAILSIZEATTEN 0.01f //β���ĳߴ���˥��
#define ISTAILSCALE 1 //β���ߴ��Ƿ�˥��
#define TRANSPARENT 1.0f //���ӵĳ�ʼ͸����
#define TRANSPARENTATTEN 0.0f //����͸���ȵ�˥��

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
	float tailSizeAtten = TAILSIZEATTEN;
	bool isTailScale = ISTAILSCALE;
	float transparent = TRANSPARENT;
	float transparentAtten = TRANSPARENTATTEN;
};
class Particle {
public:
	int ID;//����ID
	glm::vec3 dir;//���ӳ�ʼ����
	Model particleModel;//����ģ��
	Shader particleShader;//���Ƶ�ǰ���ӵ�shader
	glm::vec3 position;//���ӵĵ�ǰλ��
	glm::vec3 speed;//�����ز�ͬ������ٶ�
	glm::vec3 accelerate;//��������������ļ��ٶ�
	int life; //��������
	list<glm::vec3>tailPosition; //�洢β��������λ��
	float size; //��ʼ��scale
	float transparent; //���ӵĳ�ʼ͸����

public:
	//���캯��
	Particle() {}
	Particle(int id) {
		ID = id;
	}
	//��������
	~Particle() {}
	
};