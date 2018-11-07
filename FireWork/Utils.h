#pragma once
// 获取两点之间的距离
float getDistance(glm::vec3 p1, glm::vec3 p2 = glm::vec3(0.0f)) {
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;
	float z = p1.z - p2.z;
	return sqrt(x*x + y*y + z*z);
}

