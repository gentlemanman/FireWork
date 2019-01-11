#pragma once
//�Ƿ��ͼ
#define isScreenshot 1
//����Ƿ����
#define isMouse 0


//����ϵͳ�ĳ�ʼ����
#define NUMBER 9//���ӵ�����
#define NUMBER_BLUR 0.1//�������������Ķ���
#define DELTA_T 0.02f //ʱ����
#define INITIALSPEED 2.0f //��x��y��z����ĳ�ʼ�ٶ�
#define ISRESIST 1//�Ƿ��������
#define AX 0.0f //x�ļ��ٶȣ�������
#define AY 1.5f //y�ļ��ٶȣ������������ĺ�����
#define AZ 0.0f //z�ļ��ٶȣ�������
#define INITIALLIFE 100 //��������
#define SAVETAILNUM 15 // ����β�����ӵĸ���
#define INTERTAILNUM 30 //����������֮���ֵ������
#define SIZE 1.0f //���ӵĳ�ʼScale
#define SIZEATTEN 0.001f //����size˥��������(scale)
#define ISTAILSCALE 1 //β���ߴ��Ƿ�˥��
#define TRANSPARENT 1.0f //���ӵĳ�ʼ͸����
#define TRANSPARENTATTEN 0.001f //����͸���ȵ�˥��
#define INTERVALFRAME 3 //����֮ǰβ��λ�õļ��֡��
#define SPEEDBLUR 0.02f //�ٶȵ��Ŷ�
#define DIRBLUR 0.02f //������Ŷ�
#define LIFEBLUR 0.2 //�������Ŷ�
#define SIZEBLUR 0.2 //�ߴ���Ŷ�
#define TRANSPARENTBLUR 0.02 //͸���ȵ��Ŷ�
#define SAVE_TAIL_NUM_BLUR 0.02 // ����β�����Ӹ����Ķ���
#define RESIST_BLUR 0.2//�����������Ŷ�


#define PI 3.1415927f

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
	float saveTailNumBlur = SAVE_TAIL_NUM_BLUR;
	glm::vec3 color = glm::vec3(1.0f, 0.5f, 0.31f);
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
	list<glm::vec3> tailPosition; //�洢β��������λ��
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