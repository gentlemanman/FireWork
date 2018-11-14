#pragma once
#include <FreeImage/FreeImage.h>
//新建文件夹
#include <direct.h>
#include <io.h>
// 获取两点之间的距离
float getDistance(glm::vec3 p1, glm::vec3 p2 = glm::vec3(0.0f)) {
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;
	float z = p1.z - p2.z;
	return sqrt(x*x + y*y + z*z);
}

//保存当前窗口帧
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
		if (isSuccess == -1) cout << "未能成功创建" << dirName << endl;
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
	bool bSuccess = FreeImage_Save(FIF_PNG, bitmap, fileName.c_str(), PNG_Z_NO_COMPRESSION);
	if (bSuccess == false)
		cout << "保存失败" << endl;
	FreeImage_Unload(bitmap);
	delete[] mpixels;
}

//获取爆炸范围
float getExplosionRangle(vector<Particle> particles) {
	float range = 0;
	for (int i = 0; i < particles.size(); i++) {
		if (getDistance(particles[i].position) > range) {
			range = getDistance(particles[i].position);
		}
	}
	return range;
}

