#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out float curAlpha;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int saveNum;
uniform int interNum;
uniform float size;
uniform float alpha;
uniform vec3 tailPos[50];//保存尾部的个数
uniform bool isTailScale;//尾部是否缩放

mat4 getModel(int ID, int totalNum);
float getAlpha(int ID, int totalNum);
float getScale(int ID, int totalNum);
mat4 translate(mat4 m, vec3 v);
mat4 scale(mat4 m, vec3 v);

void main()
{
    TexCoords = aTexCoords;
    int totalNum = (saveNum - 1) * (interNum + 1);
	curAlpha = getAlpha(gl_InstanceID, totalNum); //获取当前例子alpha
    // 根据 gl_InstanceID 来获取model矩阵
    mat4 curModel = getModel(gl_InstanceID, totalNum);
    gl_Position = projection * view * curModel * vec4(aPos, 1.0);
}

mat4 getModel(int ID, int totalNum){
    int i = ID / (interNum + 1);//插值的区间
    int j = ID % (interNum + 1);//在插值区间的具体位置
    vec3 start = tailPos[i];
    vec3 end = tailPos[i + 1];
    vec3 interval = (end - start) / (interNum + 1);
    vec3 interPos = start + interval * j; //计算当前pos
    float interSize = getScale(ID, totalNum);//计算当前size
	//根据pos和size生成新的model矩阵
	mat4 resModel = translate(model, interPos);
	resModel = scale(resModel, vec3(interSize));
	return resModel;
}

float getAlpha(int ID, int totalNum){
	return alpha / totalNum * (totalNum - ID);
}

float getScale(int ID, int totalNum){
	if(isTailScale)
		return size / totalNum * (totalNum - ID);
	return size;
}

mat4 translate(mat4 m, vec3 v){
	mat4 res = m;
	res[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
	return res;
}
mat4 scale(mat4 m, vec3 v){
	mat4 res = m;
	res[0] = m[0] * v[0];
	res[1] = m[1] * v[1];
	res[2] = m[2] * v[2];
	return res;
}






