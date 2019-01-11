1.particle中头文件的参数信息
//粒子系统的初始属性
#define NUMBER 10//粒子的数量
#define NUMBER_BLUR 0.1//生成粒子数量的抖动
#define DELTA_T 0.02f //时间间隔
#define INITIALSPEED 2.0f //沿x，y，z方向的初始速度
#define ISRESIST 1;//是否添加阻力
#define AX 0.0f //x的加速度（阻力）
#define AY 0.0f //y的加速度（重力与阻力的合力）
#define AZ 0.0f //z的加速度（阻力）
#define INITIALLIFE 100 //生命周期
#define SAVETAILNUM 10 // 保存尾部粒子的个数
#define INTERTAILNUM 30 //在两个粒子之间插值的数量
#define SIZE 1.0f //粒子的初始Scale
#define SIZEATTEN 0.001f //粒子size衰减的速率(scale)
#define ISTAILSCALE 1 //尾部尺寸是否衰减
#define TRANSPARENT 1.0f //粒子的初始透明度
#define TRANSPARENTATTEN 0.001f //粒子透明度的衰减
#define INTERVALFRAME 3 //保存之前尾部位置的间隔帧数
#define SPEEDBLUR 0.02f //速度的扰动
#define DIRBLUR 0.02f //方向的扰动
#define LIFEBLUR 0.2 //生命的扰动
#define SIZEBLUR 0.2 //尺寸的扰动
#define TRANSPARENTBLUR 0.02 //透明度的扰动
#define SAVE_TAIL_NUM_BLUR 0.02 // 保存尾部粒子个数的抖动
#define RESIST_BLUR 0.8//粒子阻力的扰动
2.
//粒子系统的初始属性
#define NUMBER 9//粒子的数量
#define NUMBER_BLUR 0.1//生成粒子数量的抖动
#define DELTA_T 0.02f //时间间隔
#define INITIALSPEED 2.0f //沿x，y，z方向的初始速度
#define ISRESIST 1//是否添加阻力
#define AX 0.0f //x的加速度（阻力）
#define AY 0.0f //y的加速度（重力与阻力的合力）
#define AZ 0.0f //z的加速度（阻力）
#define INITIALLIFE 100 //生命周期
#define SAVETAILNUM 15 // 保存尾部粒子的个数
#define INTERTAILNUM 30 //在两个粒子之间插值的数量
#define SIZE 0.7f //粒子的初始Scale
#define SIZEATTEN 0.001f //粒子size衰减的速率(scale)
#define ISTAILSCALE 0 //尾部尺寸是否衰减
#define TRANSPARENT 1.0f //粒子的初始透明度
#define TRANSPARENTATTEN 0.001f //粒子透明度的衰减
#define INTERVALFRAME 3 //保存之前尾部位置的间隔帧数
#define SPEEDBLUR 0.02f //速度的扰动
#define DIRBLUR 0.02f //方向的扰动
#define LIFEBLUR 0.2 //生命的扰动
#define SIZEBLUR 0.2 //尺寸的扰动
#define TRANSPARENTBLUR 0.02 //透明度的扰动
#define SAVE_TAIL_NUM_BLUR 0.02 // 保存尾部粒子个数的抖动
#define RESIST_BLUR 0.8//粒子阻力的扰动
3.
//添加重力和粒子尾部的尺寸衰减
//粒子系统的初始属性
#define NUMBER 9//粒子的数量
#define NUMBER_BLUR 0.1//生成粒子数量的抖动
#define DELTA_T 0.02f //时间间隔
#define INITIALSPEED 2.0f //沿x，y，z方向的初始速度
#define ISRESIST 1//是否添加阻力
#define AX 0.0f //x的加速度（阻力）
#define AY 1.5f //y的加速度（重力与阻力的合力）
#define AZ 0.0f //z的加速度（阻力）
#define INITIALLIFE 100 //生命周期
#define SAVETAILNUM 15 // 保存尾部粒子的个数
#define INTERTAILNUM 30 //在两个粒子之间插值的数量
#define SIZE 1.0f //粒子的初始Scale
#define SIZEATTEN 0.001f //粒子size衰减的速率(scale)
#define ISTAILSCALE 1 //尾部尺寸是否衰减
#define TRANSPARENT 1.0f //粒子的初始透明度
#define TRANSPARENTATTEN 0.001f //粒子透明度的衰减
#define INTERVALFRAME 3 //保存之前尾部位置的间隔帧数
#define SPEEDBLUR 0.02f //速度的扰动
#define DIRBLUR 0.02f //方向的扰动
#define LIFEBLUR 0.2 //生命的扰动
#define SIZEBLUR 0.2 //尺寸的扰动
#define TRANSPARENTBLUR 0.02 //透明度的扰动
#define SAVE_TAIL_NUM_BLUR 0.02 // 保存尾部粒子个数的抖动
#define RESIST_BLUR 0.2//粒子阻力的扰动
4.
//仅添加重力没有尺寸衰减
//粒子系统的初始属性
#define NUMBER 9//粒子的数量
#define NUMBER_BLUR 0.1//生成粒子数量的抖动
#define DELTA_T 0.02f //时间间隔
#define INITIALSPEED 2.0f //沿x，y，z方向的初始速度
#define ISRESIST 1//是否添加阻力
#define AX 0.0f //x的加速度（阻力）
#define AY 1.5f //y的加速度（重力与阻力的合力）
#define AZ 0.0f //z的加速度（阻力）
#define INITIALLIFE 100 //生命周期
#define SAVETAILNUM 15 // 保存尾部粒子的个数
#define INTERTAILNUM 30 //在两个粒子之间插值的数量
#define SIZE 0.7f //粒子的初始Scale
#define SIZEATTEN 0.001f //粒子size衰减的速率(scale)
#define ISTAILSCALE 0 //尾部尺寸是否衰减
#define TRANSPARENT 1.0f //粒子的初始透明度
#define TRANSPARENTATTEN 0.001f //粒子透明度的衰减
#define INTERVALFRAME 3 //保存之前尾部位置的间隔帧数
#define SPEEDBLUR 0.02f //速度的扰动
#define DIRBLUR 0.02f //方向的扰动
#define LIFEBLUR 0.2 //生命的扰动
#define SIZEBLUR 0.2 //尺寸的扰动
#define TRANSPARENTBLUR 0.02 //透明度的扰动
#define SAVE_TAIL_NUM_BLUR 0.02 // 保存尾部粒子个数的抖动
#define RESIST_BLUR 0.2//粒子阻力的扰动