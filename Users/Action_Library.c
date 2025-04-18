#include "Action_library.h"
#include "user_servo.h"


#if 1
#include <stdio.h>

/* 告知连接器不从C库链接使用半主机的函数 */
#pragma import(__use_no_semihosting)

/* 定义 _sys_exit() 以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

/* 标准库需要的支持类型 */
struct __FILE
{
    int handle;
};

FILE __stdout;

/*  */
int fputc(int ch, FILE *stream)
{
    /* 堵塞判断串口是否发送完成 */
    while((UART7->ISR & 0X40) == 0);

    /* 串口发送完成，将该字符发送 */
    UART7->TDR = (uint8_t) ch;

    return ch;
}

#endif


//指示当前序号动作的具体地址（用于调度动作）
ServoActionSeries* Action_index[ACTION_COUNT_MAX];
int TEACHMODE = 0;//表示示教模式的开闭
int TEACH_OK = 0;//置位表示开始实际进行示教
int TEACH_FINISH = 0;//标识当前示教结束与否

//下面为实际动作序列的定义
ServoActionSeries Action_Hello;
ServoActionSeries Action_Hug;
ServoActionSeries Action_Standup;
ServoActionSeries Action_Sit2Prone;
ServoActionSeries Action_PronetoSit;
ServoActionSeries Action_Lie2Standup;
ServoActionSeries Action_Crawl;
ServoActionSeries Action_Crawlrepeat;
ServoActionSeries Action_Walk = (ServoActionSeries){
	.actionId = 1,
	
	.actions[0] = (ServoActionStep){
	.servoAngles = {0, 97, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0 },
	.stepDuration = 2
	},
	.actions[1] = (ServoActionStep){
	.servoAngles = {0, 97, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0 },
	.stepDuration = 2
	},
	.actions[2] = (ServoActionStep){
	.servoAngles = {0, 300, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0 },
	.stepDuration = 2
	},
	.actions[3] = (ServoActionStep){
	.servoAngles = {0, 266, 442, 410, 42, 796, -81, -647, -527, 218, -1082, 0, 0, 0 },
	.stepDuration = 2
	},
	.actions[4] = (ServoActionStep){
	.servoAngles = {0, 179, 535, 542, 16, 914, -87, -591, -527, 198, -946, 0, 0, 0 },
	.stepDuration = 2
	},
	.actions[5] = (ServoActionStep){
	.servoAngles = {0, 84, 614, 644, -123, 890, 100, -539, -700, 250, -915, 0, 0, 0 },
	.stepDuration = 2
	},
	.actions[6] = (ServoActionStep){
	.servoAngles = {0, 84, 614, 644, -123, 890, 100, -539, -700, 125, -755, 0, 0, 0 },
	.stepDuration = 2
	},
	.actions[7] = (ServoActionStep){
	.servoAngles = {0, 7, 563, 647, -188, 843, 96, -603, -704, -61, -850, 0, 0, 0 },
	.stepDuration = 2
	},
	.actions[8] = (ServoActionStep){
	.servoAngles = {0, 13, 644, 651, -243, 1100, 38, -600, -692, 78, -823, 0, 0, 0 },
	.stepDuration = 2
	},
	.actions[9] = (ServoActionStep){
	.servoAngles = {0, 13, 644, 651, -243, 1050, -275, -598, -692, 78, -823, 0, 0, 0 },
	.stepDuration = 2
	},
	.actions[10] = (ServoActionStep){
	.servoAngles = {0, 13, 644, 651, -243, 1050, -250, -639, -315, 78, -823, 0, 0, 0 },
	.stepDuration = 2
	},
	.actions[11] = (ServoActionStep){
	.servoAngles = {0, 19, 580, 658, -325, 900, -125, -614, -600, 250, -1050, 0, 0, 0 },
	.stepDuration = 2
	},
	.actions[12] = (ServoActionStep){
	.servoAngles = {0, 19, 580, 658, -200, 700, -125, -614, -600, 250, -1050, 0, 0, 0 },
	.stepDuration = 2
	},
	.actions[13] = (ServoActionStep){
	.servoAngles = {0, 24, 586, 662, -22, 860, 27, -639, -674, 349, -1053, 0, 0, 0 },
	.stepDuration = 2
	},
	.actions[14] = (ServoActionStep){
	.servoAngles = {0, 97, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0 },
	.stepDuration = 2
	},
	
	.emotionType = EMOTION_NEUTRAL,
	.total_step = 15,
	.totalDuration = 5000
	
	
};
ServoActionSeries Action_TEACH;
ServoActionSeries Action_Test = (ServoActionSeries){
	.actionId = 14,
	
	.actions[0] = (ServoActionStep){
	.servoAngles = {672, -5, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[1] = (ServoActionStep){
	.servoAngles = {672, -5, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[2] = (ServoActionStep){
	.servoAngles = {672, -5, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[3] = (ServoActionStep){
	.servoAngles = {672, -5, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[4] = (ServoActionStep){
	.servoAngles = {672, -3, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[5] = (ServoActionStep){
	.servoAngles = {655, -6, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[6] = (ServoActionStep){
	.servoAngles = {410, -6, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[7] = (ServoActionStep){
	.servoAngles = {744, -1, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[8] = (ServoActionStep){
	.servoAngles = {1010, -6, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[9] = (ServoActionStep){
	.servoAngles = {693, -32, 365, -37, 134, 130, 555, -118, -478, -903, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[10] = (ServoActionStep){
	.servoAngles = {377, -32, 365, -37, 134, 130, 555, -118, -478, -903, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[11] = (ServoActionStep){
	.servoAngles = {739, -79, 365, -37, 134, 130, 555, -118, -478, -903, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[12] = (ServoActionStep){
	.servoAngles = {1008, -84, 365, -37, 134, 130, 555, -118, -478, -903, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[13] = (ServoActionStep){
	.servoAngles = {605, -79, 367, -37, 134, 130, 555, -118, -478, -903, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[14] = (ServoActionStep){
	.servoAngles = {405, -47, 366, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[15] = (ServoActionStep){
	.servoAngles = {746, -122, 367, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[16] = (ServoActionStep){
	.servoAngles = {978, -107, 368, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[17] = (ServoActionStep){
	.servoAngles = {636, -71, 367, -37, 134, 130, 555, -118, -478, -904, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[18] = (ServoActionStep){
	.servoAngles = {428, -61, 367, -37, 134, 130, 555, -118, -478, -904, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[19] = (ServoActionStep){
	.servoAngles = {723, -144, 367, -37, 134, 130, 555, -118, -478, -904, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[20] = (ServoActionStep){
	.servoAngles = {826, -151, 368, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[21] = (ServoActionStep){
	.servoAngles = {450, -138, 366, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[22] = (ServoActionStep){
	.servoAngles = {381, -117, 366, -37, 134, 130, 555, -118, -478, -904, -99, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[23] = (ServoActionStep){
	.servoAngles = {724, -138, 366, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[24] = (ServoActionStep){
	.servoAngles = {742, -137, 366, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[25] = (ServoActionStep){
	.servoAngles = {347, -110, 366, -37, 134, 130, 555, -118, -478, -903, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[26] = (ServoActionStep){
	.servoAngles = {589, -102, 366, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[27] = (ServoActionStep){
	.servoAngles = {899, -105, 366, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[28] = (ServoActionStep){
	.servoAngles = {691, -113, 366, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[29] = (ServoActionStep){
	.servoAngles = {411, -95, 366, -37, 134, 130, 555, -118, -478, -903, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[30] = (ServoActionStep){
	.servoAngles = {543, -84, 365, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[31] = (ServoActionStep){
	.servoAngles = {863, -85, 365, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[32] = (ServoActionStep){
	.servoAngles = {923, -85, 365, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[33] = (ServoActionStep){
	.servoAngles = {678, -87, 365, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	.actions[34] = (ServoActionStep){
	.servoAngles = {551, -85, 365, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0 },
	.stepDuration = 2
	},
	
	.emotionType = EMOTION_NEUTRAL,
	.total_step = 35,
	.totalDuration = 5000
	
	
};


ServoActionSeries Active_Wave = (ServoActionSeries){
	.actionId = 3,
	.actions[0] = (ServoActionStep){
	.servoAngles = {0, 169, 613, 559, -49, 896, -266, -540, -552, 90, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[1] = (ServoActionStep){
	.servoAngles = {0, 169, 613, 559, -49, 896, -266, -540, -552, 90, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[2] = (ServoActionStep){
	.servoAngles = {0, 169, 614, 559, -49, 896, -266, -540, -552, 90, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[3] = (ServoActionStep){
	.servoAngles = {0, 169, 615, 559, -49, 898, -266, -547, -553, 89, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[4] = (ServoActionStep){
	.servoAngles = {0, 169, 615, 559, -49, 896, -266, -548, -553, 89, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[5] = (ServoActionStep){
	.servoAngles = {0, 169, 615, 559, -49, 896, -276, -541, -548, 89, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[6] = (ServoActionStep){
	.servoAngles = {0, 168, 612, 559, -59, 896, -291, -540, -551, 94, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[7] = (ServoActionStep){
	.servoAngles = {0, 168, 611, 557, -60, 896, -508, -543, -553, 95, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[8] = (ServoActionStep){
	.servoAngles = {0, 168, 614, 559, -50, 896, -777, -541, -553, 91, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[9] = (ServoActionStep){
	.servoAngles = {0, 175, 615, 561, -48, 897, -766, -540, -553, 88, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[10] = (ServoActionStep){
	.servoAngles = {0, 176, 616, 561, -48, 897, -523, -541, -553, 88, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[11] = (ServoActionStep){
	.servoAngles = {0, 176, 616, 561, -48, 897, -226, -547, -553, 88, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[12] = (ServoActionStep){
	.servoAngles = {0, 176, 616, 561, -49, 897, -8, -549, -553, 88, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[13] = (ServoActionStep){
	.servoAngles = {0, 169, 615, 561, -59, 897, -42, -549, -553, 94, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[14] = (ServoActionStep){
	.servoAngles = {0, 169, 608, 560, -60, 897, -310, -549, -554, 95, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[15] = (ServoActionStep){
	.servoAngles = {0, 169, 614, 560, -58, 897, -628, -549, -554, 95, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[16] = (ServoActionStep){
	.servoAngles = {0, 175, 616, 561, -49, 898, -759, -549, -554, 90, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[17] = (ServoActionStep){
	.servoAngles = {0, 176, 616, 561, -48, 898, -630, -548, -554, 88, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[18] = (ServoActionStep){
	.servoAngles = {0, 176, 616, 561, -48, 898, -349, -548, -554, 88, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[19] = (ServoActionStep){
	.servoAngles = {0, 176, 616, 561, -49, 899, -85, -549, -554, 88, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[20] = (ServoActionStep){
	.servoAngles = {0, 170, 616, 561, -49, 899, -13, -550, -554, 89, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[21] = (ServoActionStep){
	.servoAngles = {0, 169, 615, 561, -59, 899, -169, -549, -554, 94, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[22] = (ServoActionStep){
	.servoAngles = {0, 169, 615, 561, -59, 899, -470, -541, -554, 94, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[23] = (ServoActionStep){
	.servoAngles = {0, 169, 615, 561, -50, 899, -736, -550, -554, 94, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[24] = (ServoActionStep){
	.servoAngles = {0, 169, 616, 555, -55, 899, -785, -581, -564, 95, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[25] = (ServoActionStep){
	.servoAngles = {0, 175, 616, 555, -49, 898, -783, -765, -598, 90, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[26] = (ServoActionStep){
	.servoAngles = {0, 176, 616, 561, -48, 898, -710, -800, -597, 89, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[27] = (ServoActionStep){
	.servoAngles = {0, 176, 616, 561, -48, 898, -464, -799, -596, 89, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[28] = (ServoActionStep){
	.servoAngles = {0, 176, 616, 561, -49, 897, -242, -798, -596, 89, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[29] = (ServoActionStep){
	.servoAngles = {0, 176, 616, 561, -49, 897, -117, -798, -596, 89, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[30] = (ServoActionStep){
	.servoAngles = {0, 172, 615, 561, -50, 897, -106, -799, -597, 91, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[31] = (ServoActionStep){
	.servoAngles = {0, 171, 615, 561, -50, 897, -106, -799, -597, 90, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[32] = (ServoActionStep){
	.servoAngles = {0, 170, 615, 561, -50, 897, -106, -799, -597, 90, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[33] = (ServoActionStep){
	.servoAngles = {0, 169, 615, 561, -50, 897, -106, -799, -597, 90, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[34] = (ServoActionStep){
	.servoAngles = {0, 169, 608, 561, -49, 897, -106, -799, -597, 90, -827, 0, 0, 0 },
		.stepDuration = 2
	},
	.emotionType = EMOTION_NEUTRAL,
	.total_step = 35,
	.totalDuration = 5000

};

ServoActionSeries Active_Sit = (ServoActionSeries){
	.actionId = 4,
	.actions[0] = (ServoActionStep){
	.servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[1] = (ServoActionStep){
	.servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[2] = (ServoActionStep){
	.servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[3] = (ServoActionStep){
	.servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[4] = (ServoActionStep){
	.servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[5] = (ServoActionStep){
	.servoAngles = {0, 117, 892, 644, 388, 903, -132, -798, -595, -361, -835, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[6] = (ServoActionStep){
	.servoAngles = {0, 121, 892, 644, 376, 904, -134, -797, -595, -349, -838, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[7] = (ServoActionStep){
	.servoAngles = {0, 122, 891, 644, 303, 904, -135, -797, -595, -310, -838, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[8] = (ServoActionStep){
	.servoAngles = {0, 122, 891, 644, 285, 904, -135, -797, -595, -247, -837, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[9] = (ServoActionStep){
	.servoAngles = {0, 122, 891, 644, 234, 904, -135, -797, -595, -189, -838, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[10] = (ServoActionStep){
	.servoAngles = {0, 122, 891, 644, 100, 904, -135, -803, -595, -78, -830, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[11] = (ServoActionStep){
	.servoAngles = {0, 121, 891, 644, -55, 904, -135, -803, -595, 89, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[12] = (ServoActionStep){
	.servoAngles = {0, 121, 891, 644, -73, 904, -135, -804, -595, 105, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[13] = (ServoActionStep){
	.servoAngles = {0, 121, 891, 644, -72, 903, -134, -802, -595, 105, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[14] = (ServoActionStep){
	.servoAngles = {0, 120, 891, 644, -72, 903, -134, -801, -595, 107, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[15] = (ServoActionStep){
	.servoAngles = {0, 116, 887, 642, -72, 899, -133, -797, -595, 107, -827, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[16] = (ServoActionStep){
	.servoAngles = {0, 123, 709, 642, -72, 895, -136, -796, -599, 110, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[17] = (ServoActionStep){
	.servoAngles = {0, 123, 486, 642, -72, 895, -136, -796, -600, 106, -836, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[18] = (ServoActionStep){
	.servoAngles = {0, 171, 422, 642, -72, 895, -134, -803, -596, 105, -837, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[19] = (ServoActionStep){
	.servoAngles = {0, 188, 425, 643, -72, 896, -133, -806, -596, 106, -831, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[20] = (ServoActionStep){
	.servoAngles = {0, 279, 423, 643, -72, 895, -136, -795, -594, 106, -836, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[21] = (ServoActionStep){
	.servoAngles = {0, 445, 424, 643, -72, 895, -168, -795, -594, 106, -836, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[22] = (ServoActionStep){
	.servoAngles = {0, 447, 425, 643, -72, 895, -356, -724, -593, 106, -836, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[23] = (ServoActionStep){
	.servoAngles = {0, 447, 426, 643, -72, 895, -461, -670, -593, 106, -836, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[24] = (ServoActionStep){
	.servoAngles = {0, 447, 427, 643, -72, 895, -456, -674, -595, 106, -836, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[25] = (ServoActionStep){
	.servoAngles = {0, 446, 481, 642, -72, 895, -436, -674, -595, 106, -836, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[26] = (ServoActionStep){
	.servoAngles = {0, 446, 653, 642, -72, 895, -297, -710, -602, 106, -835, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[27] = (ServoActionStep){
	.servoAngles = {0, 445, 726, 642, -72, 895, -244, -850, -602, 106, -835, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[28] = (ServoActionStep){
	.servoAngles = {0, 445, 752, 642, -72, 895, -213, -884, -604, 106, -835, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[29] = (ServoActionStep){
	.servoAngles = {0, 446, 826, 642, -72, 895, -218, -819, -594, 106, -833, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[30] = (ServoActionStep){
	.servoAngles = {0, 446, 846, 642, -75, 895, -236, -589, -594, 106, -829, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[31] = (ServoActionStep){
	.servoAngles = {0, 445, 839, 642, -73, 895, -380, -454, -594, 106, -829, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[32] = (ServoActionStep){
	.servoAngles = {0, 428, 836, 641, -73, 895, -516, -456, -603, 107, -829, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[33] = (ServoActionStep){
	.servoAngles = {0, 308, 835, 604, -73, 895, -570, -562, -604, 108, -829, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[34] = (ServoActionStep){
	.servoAngles = {0, 307, 835, 542, -72, 894, -545, -758, -603, 106, -829, 0, 0, 0 },
		.stepDuration = 2
	},
	.emotionType = EMOTION_NEUTRAL,
		.total_step = 35,
	.totalDuration = 5000

};

ServoActionSeries Active_SittoEat = (ServoActionSeries){
	.actionId = 5,
	.actions[0] = (ServoActionStep){
	.servoAngles = {0, 97, 858, 557, -53, 897, -41, -810, -600, 94, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[1] = (ServoActionStep){
	.servoAngles = {0, 97, 858, 557, -53, 897, -41, -810, -600, 94, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[2] = (ServoActionStep){
	.servoAngles = {0, 97, 859, 557, -53, 897, -41, -810, -600, 94, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[3] = (ServoActionStep){
	.servoAngles = {0, 97, 858, 552, -53, 897, -41, -811, -600, 94, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[4] = (ServoActionStep){
	.servoAngles = {0, 97, 859, 552, -53, 897, -40, -811, -600, 93, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[5] = (ServoActionStep){
	.servoAngles = {0, 94, 858, 551, -47, 896, -35, -809, -600, 83, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[6] = (ServoActionStep){
	.servoAngles = {0, 94, 857, 551, 22, 896, 65, -808, -600, 20, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[7] = (ServoActionStep){
	.servoAngles = {0, 50, 856, 550, 53, 896, 74, -808, -600, -23, -832, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[8] = (ServoActionStep){
	.servoAngles = {0, 34, 856, 550, 45, 896, 72, -808, -600, -18, -831, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[9] = (ServoActionStep){
	.servoAngles = {0, 72, 856, 550, 44, 899, 70, -808, -600, -17, -831, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[10] = (ServoActionStep){
	.servoAngles = {0, 234, 856, 550, 44, 899, -108, -808, -600, -17, -831, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[11] = (ServoActionStep){
	.servoAngles = {0, 380, 856, 550, 44, 899, -339, -808, -600, -17, -831, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[12] = (ServoActionStep){
	.servoAngles = {0, 401, 857, 550, 44, 899, -380, -817, -600, -17, -831, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[13] = (ServoActionStep){
	.servoAngles = {0, 374, 857, 550, 70, 896, -379, -817, -599, -37, -829, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[14] = (ServoActionStep){
	.servoAngles = {0, 266, 857, 550, 103, 899, -289, -818, -599, -57, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[15] = (ServoActionStep){
	.servoAngles = {0, 180, 857, 550, 104, 899, -76, -809, -599, -57, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[16] = (ServoActionStep){
	.servoAngles = {0, 149, 858, 550, 94, 899, -33, -809, -599, -53, -832, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[17] = (ServoActionStep){
	.servoAngles = {0, 275, 857, 550, 94, 899, -225, -815, -599, -48, -832, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[18] = (ServoActionStep){
	.servoAngles = {0, 519, 857, 550, 94, 899, -404, -818, -599, -48, -832, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[19] = (ServoActionStep){
	.servoAngles = {0, 554, 857, 550, 105, 900, -504, -819, -599, -59, -830, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[20] = (ServoActionStep){
	.servoAngles = {0, 551, 858, 550, 106, 899, -434, -820, -599, -81, -832, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[21] = (ServoActionStep){
	.servoAngles = {0, 405, 868, 550, 106, 898, -254, -820, -599, -97, -832, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[22] = (ServoActionStep){
	.servoAngles = {0, 195, 867, 550, 125, 900, -75, -819, -599, -97, -830, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[23] = (ServoActionStep){
	.servoAngles = {0, 155, 867, 555, 133, 901, -12, -810, -599, -92, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[24] = (ServoActionStep){
	.servoAngles = {0, 180, 865, 556, 133, 901, -167, -815, -599, -90, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[25] = (ServoActionStep){
	.servoAngles = {0, 332, 866, 552, 133, 901, -258, -819, -599, -90, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[26] = (ServoActionStep){
	.servoAngles = {0, 466, 867, 555, 133, 901, -397, -820, -605, -90, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[27] = (ServoActionStep){
	.servoAngles = {0, 465, 868, 552, 133, 901, -479, -820, -601, -90, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[28] = (ServoActionStep){
	.servoAngles = {0, 465, 868, 550, 145, 902, -458, -820, -599, -115, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[29] = (ServoActionStep){
	.servoAngles = {0, 395, 868, 550, 177, 899, -327, -820, -599, -169, -829, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[30] = (ServoActionStep){
	.servoAngles = {0, 260, 868, 550, 220, 902, -231, -819, -599, -175, -828, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[31] = (ServoActionStep){
	.servoAngles = {0, 244, 867, 550, 222, 901, -125, -819, -599, -176, -829, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[32] = (ServoActionStep){
	.servoAngles = {0, 244, 867, 555, 223, 900, -97, -819, -599, -176, -832, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[33] = (ServoActionStep){
	.servoAngles = {0, 254, 867, 556, 215, 900, -111, -819, -600, -175, -833, 0, 0, 0 },
		.stepDuration = 2
	},
		.actions[34] = (ServoActionStep){
	.servoAngles = {0, 295, 859, 551, 213, 900, -324, -819, -600, -167, -831, 0, 0, 0 },
		.stepDuration = 2
	},
	.emotionType = EMOTION_NEUTRAL,
		.total_step = 35,
	.totalDuration = 5000

};

ServoActionSeries Active_Micromove_4 = (ServoActionSeries){
		
};

ServoActionSeries Active_Micromove_5 = (ServoActionSeries){
		
};

ServoActionSeries Active_Micromove_6 = (ServoActionSeries){
		
};

ServoActionSeries Active_Micromove_7 = (ServoActionSeries){
		
};
//开启示教模式
void Action_Teachmode_Init(void)
{
	TEACHMODE = 1;
}
//示教结束后向上位机发送的代码格式（可设置）
void Action_Teachmode(void)
{
	for(int si = 0;si < 35;si++)
	{
		printf("	.actions[%d] = (ServoActionStep){",si);
		printf("\r\n");
		printf(".servoAngles = {");
		for(int i = 0;i < 13;i++)
		{
			printf("%d, ", Action_TEACH.actions[si].servoAngles[i]);
		}
		printf("%d ", Action_TEACH.actions[si].servoAngles[13]);
		printf("},\r\n	.stepDuration = 2\r\n");
		printf("},\r\n");
	}
	printf("	.emotionType = EMOTION_NEUTRAL,\r\n		.total_step = 35,\r\n	.totalDuration = 5000\r\n");
}
//动作库初始化函数
void Action_init(void)
{
	Action_index[0] = &Action_TEACH;
	Action_index[1] = &Action_Walk;
	Action_index[2] = &Action_Hug;
	Action_index[3] = &Active_Wave;
	Action_index[4] = &Active_Sit;
	Action_index[5] = &Active_SittoEat;
	Action_index[6] = &Action_Lie2Standup;
	Action_index[7] = &Action_Crawl;
	Action_index[10] = &Action_Crawlrepeat;
	Action_index[14] = &Action_Test;
	
	Action_TEACH.actionId = 0;
	Action_TEACH.emotionType = EMOTION_NEUTRAL;
	Action_TEACH.totalDuration = 5000;
	Action_TEACH.total_step = TEACH_TOTAL_STEP;
	
}



