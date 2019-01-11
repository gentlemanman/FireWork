#pragma once
#include <FreeImage/FreeImage.h>
//�½��ļ���
#include <direct.h>
#include <io.h>

class cRandom {
public:
	cRandom(int x, double y) :seed(x), random(y) {};
	cRandom() :seed(0), random(0) {};
	int seed;
	double random;
};

cRandom my_random(int z) {
	const int m = pow(2, 31) - 1;
	const int a = 16807;
	const int q = 127773;
	const int r = 2836;
	int temp = a * (z % q) - r * (z / q);
	if (temp < 0) {
		temp = temp + m;
	}
	z = temp;
	double t = z * 1.0 / m;
	cRandom cr;
	cr.random = t;
	cr.seed = z;
	return cr;
}

//���浱ǰ����֡
void saveImage(bool isRGB, int example, int num, int width, int height) {
	stringstream ss1;
	ss1 << example;
	string example_str = ss1.str();
	stringstream ss2;
	ss2 << num;
	string num_str = ss2.str();
	string dirName;
	if (isRGB) {
		dirName = "Output_rgb/rgb" + example_str;
	}
	else {
		dirName = "Output_depth/depth" + example_str;
	}
	if (_access(dirName.c_str(), 0) == -1) {
		int isSuccess = _mkdir(dirName.c_str());
		if (isSuccess == -1) cout << "δ�ܳɹ�����" << dirName << endl;
	}
	string fileName = dirName + "/" + num_str + ".png";
	unsigned char *mpixels = new unsigned char[width * height * 4];
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, mpixels);
	glReadBuffer(GL_BACK);
	for (int i = 0; i < (int)width * height * 4; i += 4) {
		mpixels[i] ^= mpixels[i + 2] ^= mpixels[i] ^= mpixels[i + 2];
	}
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32, 8, 8, 8);
	for (int y = 0; y < FreeImage_GetHeight(bitmap); y++)
	{
		BYTE *bits = FreeImage_GetScanLine(bitmap, y);
		for (int x = 0; x < FreeImage_GetWidth(bitmap); x++)
		{
			bits[0] = mpixels[(y * width + x) * 4 + 0];
			bits[1] = mpixels[(y * width + x) * 4 + 1];
			bits[2] = mpixels[(y * width + x) * 4 + 2];
			bits[3] = 255;
			bits += 4;
		}
	}
	bool bSuccess = FreeImage_Save(FIF_PNG, bitmap, fileName.c_str(), PNG_Z_DEFAULT_COMPRESSION);
	if (bSuccess == false)
		cout << "����ʧ��" << endl;
	FreeImage_Unload(bitmap);
	delete[] mpixels;
}

//��ȡ[-1, 1]֮��������
float getUnitRand() {
	return rand() % 10000 / 10000.0 * 2.0 - 1;
}

//��ȡһ���������ɫ
glm::vec3 getRandColor() {
	glm::vec3 color;
	// ��ȡ���Ƚϴ����ɫ���
	while (true) {
		float R = getUnitRand() / 2.0 + 0.5;
		float G = getUnitRand() / 2.0 + 0.5;
		float B = getUnitRand() / 2.0 + 0.5;
		float Y = 0.299*R + 0.587*G + 0.114*B;//��������
		if (Y >= 0.5) {
			color = glm::vec3(R, G, B);
			break;//����������������
		}
	}
	return color;
}

//��ȡ�������λ��
vector<glm::vec3> getSpherePoint(int num) {
	vector<glm::vec3> ans;
	srand(time(0));
	int z1 = rand();
	srand(z1);
	int z2 = rand();
	cRandom sita(z1, 0.0);
	cRandom pesi(z2, 0.0);
	for (int i = 0; i < num; ++i) {
		sita = my_random(pesi.seed);
		pesi = my_random(sita.seed);
		double u = 2 * sita.random - 1.0;
		double v = 2 * pesi.random - 1.0;
		double r2 = pow(u, 2) + pow(v, 2);
		if (r2 < 1) {
			double x = 2 * u * sqrt(1 - r2);
			double y = 2 * v * sqrt(1 - r2);
			double z = 1 - 2 * r2;
			ans.push_back(glm::vec3(x, y, z));
		}
		else {
			i--;
		}
	}
	return ans;
}

// ��ȡ����֮��ľ���
float getDistance(glm::vec3 p1, glm::vec3 p2 = glm::vec3(0.0f)) {
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;
	float z = p1.z - p2.z;
	return sqrt(x*x + y*y + z*z);
}

//��ȡ��ը��Χ
float getExplosionRangle(vector<Particle> particles) {
	float range = 0;
	for (int i = 0; i < particles.size(); i++) {
		if (getDistance(particles[i].position) > range) {
			range = getDistance(particles[i].position);
		}
	}
	return range;
}

//���Χ�����ĵ���ת��ȡ�µ�λ��
glm::vec3 getCameraRotatePoint(glm::vec3 camPos) {
	float x = camPos.x;
	float z = camPos.z;
	// Χ�����ĵ���ת������תx��z����
	//�Ȼ�ȡһ���������ת����(-pi, pi)
	float theta = getUnitRand() * PI;
	float x1 = x * cos(theta) - z * sin(theta);
	float z1 = x * sin(theta) + z * cos(theta);
	return glm::vec3(x1, camPos.y, z1);
}



