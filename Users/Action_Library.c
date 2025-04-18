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
<<<<<<< Updated upstream
	while ((UART4->ISR & 0X40) == 0)
		;

	/* 串口发送完成，将该字符发送 */
	UART4->TDR = (uint8_t)ch;
=======
	while ((UART7->ISR & 0X40) == 0)
		;

	/* 串口发送完成，将该字符发送 */
	UART7->TDR = (uint8_t)ch;
>>>>>>> Stashed changes

	return ch;
}

#endif

// 指示当前序号动作的具体地址（用于调度动作）
ServoActionSeries *Action_index[ACTION_COUNT_MAX];
int TEACHMODE = 0;	  // 表示示教模式的开闭
int TEACH_OK = 0;	  // 置位表示开始实际进行示教
int TEACH_FINISH = 0; // 标识当前示教结束与否

/*
<<<<<<< Updated upstream
Action_index[0] = &Action_TEACH;
	Action_index[1] = &Action_Walk;
	Action_index[2] = &Action_Hug;
	Action_index[3] = &Action_Standup;
	Action_index[4] = &Action_Sit2Prone;
	Action_index[5] = &Action_PronetoSit;
	Action_index[6] = &Action_Lie2Standup;
	Action_index[7] = &Action_Crawl;
	Action_index[8] = &Action_Crawlrepeat;
	Action_index[9] = &Action_Test;
*/

// 下面为实际动作序列的定义
ServoActionSeries Action_LieDown = (ServoActionSeries){
	.actionId = 12,
	.ifNeedBezier = 1,
	.total_step = 35,
	.emotionType = EMOTION_NEUTRAL,
	.totalDuration = 2000,
	.endservoAngles = {0, 662, 778, 432, -843, 1044, -720, -678, -564, 867, -1096, 0, 0, 0},

	//	.endservoAngles = {0, -837, 621, 529, -62, 1056, 792, -657, -462, 96, -1097, 0, 0, 0},//坐

	//	.actions[0] = (ServoActionStep){
	//		.servoAngles = {0, -10, -402, 71, -114, 16, -3, 206, -27, -19, -242, 0, 0, 0},
	//		.stepDuration = 2},//大字躺
}; // 趴着

ServoActionSeries Action_Unit1 = (ServoActionSeries){
	.actionId = 10,
	.ifNeedBezier = 1,
	.emotionType = EMOTION_NEUTRAL,
	.totalDuration = 2000,
	.endservoAngles = {0, -837, 621, 529, -62, 1056, 792, -657, -462, 96, -1097, 0, 0, 0},
}; //

ServoActionSeries Action_UnitTest = (ServoActionSeries){
	.actionId = 11,
	.ifNeedBezier = 1,
	.total_step = 25,
	.emotionType = EMOTION_NEUTRAL,
	.totalDuration = 2000,
	.endservoAngles = {0, -837, 621, 529, -62, 1056, 792, -657, -462, 96, -1097, 0, 0, 0}}; // 坐
//	.endservoAngles = {0, 721, 688, 570, -809, 1031, -735, -821, -214, 895, -1007, 0, 0, 0}}; // 坐起来

=======
typedef struct
{
	ServoActionSeries motion[MAX_NUM_MOTION];
	uint32_t point_total;
	uint32_t point_iter;
} Motion_t;
*/

Motion_t _Action_TEACH = {
	.point_total = 1,
	.motion = {
		{.actionId = 0, // 单个动作的id
		 .ifNeedBezier = 0,
		 .emotionType = EMOTION_NEUTRAL,
		 .total_step = 35,
		 .totalDuration = 5000},
	}};

Motion_t _Action_Walk = {
	.point_total = 1,
	.motion = {
		{.actionId = 1, // 单个动作的id
		 .ifNeedBezier = 0,
		 .actions[0].servoAngles = {0, 97, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0},
		 .actions[1].servoAngles = {0, 97, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0},
		 .actions[2].servoAngles = {0, 300, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0},
		 .actions[3].servoAngles = {0, 266, 442, 410, 42, 796, -81, -647, -527, 218, -1082, 0, 0, 0},
		 .actions[4].servoAngles = {0, 179, 535, 542, 16, 914, -87, -591, -527, 198, -946, 0, 0, 0},
		 .actions[5].servoAngles = {0, 84, 614, 644, -123, 890, 100, -539, -700, 250, -915, 0, 0, 0},
		 .actions[6].servoAngles = {0, 84, 614, 644, -123, 890, 100, -539, -700, 125, -755, 0, 0, 0},
		 .actions[7].servoAngles = {0, 7, 563, 647, -188, 843, 96, -603, -704, -61, -850, 0, 0, 0},
		 .actions[8].servoAngles = {0, 13, 644, 651, -243, 1100, 38, -600, -692, 78, -823, 0, 0, 0},
		 .actions[9].servoAngles = {0, 13, 644, 651, -243, 1050, -275, -598, -692, 78, -823, 0, 0, 0},
		 .actions[10].servoAngles = {0, 13, 644, 651, -243, 1050, -250, -639, -315, 78, -823, 0, 0, 0},
		 .actions[11].servoAngles = {0, 19, 580, 658, -325, 900, -125, -614, -600, 250, -1050, 0, 0, 0},
		 .actions[12].servoAngles = {0, 19, 580, 658, -200, 700, -125, -614, -600, 250, -1050, 0, 0, 0},
		 .actions[13].servoAngles = {0, 24, 586, 662, -22, 860, 27, -639, -674, 349, -1053, 0, 0, 0},
		 .actions[14].servoAngles = {0, 97, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0},
		 .emotionType = EMOTION_NEUTRAL,
		 .total_step = 15,
		 .totalDuration = 5000},
	}};

Motion_t _Active_Wave = {
	.point_total = 1,
	.motion = {
		{.actionId = 2, // 单个动作的id
		 .ifNeedBezier = 0,
		 .actions[0].servoAngles = {0, 169, 613, 559, -49, 896, -266, -540, -552, 90, -827, 0, 0, 0},
		 .actions[1].servoAngles = {0, 169, 613, 559, -49, 896, -266, -540, -552, 90, -827, 0, 0, 0},
		 .actions[2].servoAngles = {0, 169, 614, 559, -49, 896, -266, -540, -552, 90, -827, 0, 0, 0},
		 .actions[3].servoAngles = {0, 169, 615, 559, -49, 898, -266, -547, -553, 89, -827, 0, 0, 0},
		 .actions[4].servoAngles = {0, 169, 615, 559, -49, 896, -266, -548, -553, 89, -827, 0, 0, 0},
		 .actions[5].servoAngles = {0, 169, 615, 559, -49, 896, -276, -541, -548, 89, -827, 0, 0, 0},
		 .actions[6].servoAngles = {0, 168, 612, 559, -59, 896, -291, -540, -551, 94, -827, 0, 0, 0},
		 .actions[7].servoAngles = {0, 168, 611, 557, -60, 896, -508, -543, -553, 95, -827, 0, 0, 0},
		 .actions[8].servoAngles = {0, 168, 614, 559, -50, 896, -777, -541, -553, 91, -827, 0, 0, 0},
		 .actions[9].servoAngles = {0, 175, 615, 561, -48, 897, -766, -540, -553, 88, -827, 0, 0, 0},
		 .actions[10].servoAngles = {0, 176, 616, 561, -48, 897, -523, -541, -553, 88, -827, 0, 0, 0},
		 .actions[11].servoAngles = {0, 176, 616, 561, -48, 897, -226, -547, -553, 88, -827, 0, 0, 0},
		 .actions[12].servoAngles = {0, 176, 616, 561, -49, 897, -8, -549, -553, 88, -827, 0, 0, 0},
		 .actions[13].servoAngles = {0, 169, 615, 561, -59, 897, -42, -549, -553, 94, -827, 0, 0, 0},
		 .actions[14].servoAngles = {0, 169, 608, 560, -60, 897, -310, -549, -554, 95, -827, 0, 0, 0},
		 .actions[15].servoAngles = {0, 169, 614, 560, -58, 897, -628, -549, -554, 95, -827, 0, 0, 0},
		 .actions[16].servoAngles = {0, 175, 616, 561, -49, 898, -759, -549, -554, 90, -827, 0, 0, 0},
		 .actions[17].servoAngles = {0, 176, 616, 561, -48, 898, -630, -548, -554, 88, -827, 0, 0, 0},
		 .actions[18].servoAngles = {0, 176, 616, 561, -48, 898, -349, -548, -554, 88, -827, 0, 0, 0},
		 .actions[19].servoAngles = {0, 176, 616, 561, -49, 899, -85, -549, -554, 88, -827, 0, 0, 0},
		 .actions[20].servoAngles = {0, 170, 616, 561, -49, 899, -13, -550, -554, 89, -827, 0, 0, 0},
		 .actions[21].servoAngles = {0, 169, 615, 561, -59, 899, -169, -549, -554, 94, -827, 0, 0, 0},
		 .actions[22].servoAngles = {0, 169, 615, 561, -59, 899, -470, -541, -554, 94, -827, 0, 0, 0},
		 .actions[23].servoAngles = {0, 169, 615, 561, -50, 899, -736, -550, -554, 94, -827, 0, 0, 0},
		 .actions[24].servoAngles = {0, 169, 616, 555, -55, 899, -785, -581, -564, 95, -827, 0, 0, 0},
		 .actions[25].servoAngles = {0, 175, 616, 555, -49, 898, -783, -765, -598, 90, -827, 0, 0, 0},
		 .actions[26].servoAngles = {0, 176, 616, 561, -48, 898, -710, -800, -597, 89, -827, 0, 0, 0},
		 .actions[27].servoAngles = {0, 176, 616, 561, -48, 898, -464, -799, -596, 89, -827, 0, 0, 0},
		 .actions[28].servoAngles = {0, 176, 616, 561, -49, 897, -242, -798, -596, 89, -827, 0, 0, 0},
		 .actions[29].servoAngles = {0, 176, 616, 561, -49, 897, -117, -798, -596, 89, -827, 0, 0, 0},
		 .actions[30].servoAngles = {0, 172, 615, 561, -50, 897, -106, -799, -597, 91, -827, 0, 0, 0},
		 .actions[31].servoAngles = {0, 171, 615, 561, -50, 897, -106, -799, -597, 90, -827, 0, 0, 0},
		 .actions[32].servoAngles = {0, 170, 615, 561, -50, 897, -106, -799, -597, 90, -827, 0, 0, 0},
		 .actions[33].servoAngles = {0, 169, 615, 561, -50, 897, -106, -799, -597, 90, -827, 0, 0, 0},
		 .actions[34].servoAngles = {0, 169, 608, 561, -49, 897, -106, -799, -597, 90, -827, 0, 0, 0},
		 .emotionType = EMOTION_NEUTRAL,
		 .total_step = 35,
		 .totalDuration = 5000},
	}};

Motion_t _Action_Standup = {
	.point_total = 2,
	.motion = {
		{
			.actionId = 3, // 单个动作的id
		 .ifNeedBezier = 1,
		 .total_step = 35,
		 .emotionType = EMOTION_NEUTRAL,
		 .totalDuration = 2000,
		 .endservoAngles = {0, 20, -406, 325, -543, 629, -73, 539, -442, 651, -629, 0, 0, 0}},
		{
			.actionId = 4, // 单个动作的id
		 .ifNeedBezier = 1,
		 .total_step = 35,
		 .emotionType = EMOTION_NEUTRAL,
		 .totalDuration = 2000,
		 .endservoAngles = {0, 19, 420, 318, -302, 866, -59, -421, -350, 347, -841, 0, 0, 0}}}};

Motion_t _Active_Sit = {
	.point_total = 1,
	.motion = {
		{.actionId = 5, // 单个动作的id
		 .ifNeedBezier = 0,
		 .actions[0].servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -827, 0, 0, 0},
		 .actions[1].servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -827, 0, 0, 0},
		 .actions[2].servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -827, 0, 0, 0},
		 .actions[3].servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -827, 0, 0, 0},
		 .actions[4].servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -828, 0, 0, 0},
		 .actions[5].servoAngles = {0, 117, 892, 644, 388, 903, -132, -798, -595, -361, -835, 0, 0, 0},
		 .actions[6].servoAngles = {0, 121, 892, 644, 376, 904, -134, -797, -595, -349, -838, 0, 0, 0},
		 .actions[7].servoAngles = {0, 122, 891, 644, 303, 904, -135, -797, -595, -310, -838, 0, 0, 0},
		 .actions[8].servoAngles = {0, 122, 891, 644, 285, 904, -135, -797, -595, -247, -837, 0, 0, 0},
		 .actions[9].servoAngles = {0, 122, 891, 644, 234, 904, -135, -797, -595, -189, -838, 0, 0, 0},
		 .actions[10].servoAngles = {0, 122, 891, 644, 100, 904, -135, -803, -595, -78, -830, 0, 0, 0},
		 .actions[11].servoAngles = {0, 121, 891, 644, -55, 904, -135, -803, -595, 89, -828, 0, 0, 0},
		 .actions[12].servoAngles = {0, 121, 891, 644, -73, 904, -135, -804, -595, 105, -828, 0, 0, 0},
		 .actions[13].servoAngles = {0, 121, 891, 644, -72, 903, -134, -802, -595, 105, -828, 0, 0, 0},
		 .actions[14].servoAngles = {0, 120, 891, 644, -72, 903, -134, -801, -595, 107, -827, 0, 0, 0},
		 .actions[15].servoAngles = {0, 116, 887, 642, -72, 899, -133, -797, -595, 107, -827, 0, 0, 0},
		 .actions[16].servoAngles = {0, 123, 709, 642, -72, 895, -136, -796, -599, 110, -828, 0, 0, 0},
		 .actions[17].servoAngles = {0, 123, 486, 642, -72, 895, -136, -796, -600, 106, -836, 0, 0, 0},
		 .actions[18].servoAngles = {0, 171, 422, 642, -72, 895, -134, -803, -596, 105, -837, 0, 0, 0},
		 .actions[19].servoAngles = {0, 188, 425, 643, -72, 896, -133, -806, -596, 106, -831, 0, 0, 0},
		 .actions[20].servoAngles = {0, 279, 423, 643, -72, 895, -136, -795, -594, 106, -836, 0, 0, 0},
		 .actions[21].servoAngles = {0, 445, 424, 643, -72, 895, -168, -795, -594, 106, -836, 0, 0, 0},
		 .actions[22].servoAngles = {0, 447, 425, 643, -72, 895, -356, -724, -593, 106, -836, 0, 0, 0},
		 .actions[23].servoAngles = {0, 447, 426, 643, -72, 895, -461, -670, -593, 106, -836, 0, 0, 0},
		 .actions[24].servoAngles = {0, 447, 427, 643, -72, 895, -456, -674, -595, 106, -836, 0, 0, 0},
		 .actions[25].servoAngles = {0, 446, 481, 642, -72, 895, -436, -674, -595, 106, -836, 0, 0, 0},
		 .actions[26].servoAngles = {0, 446, 653, 642, -72, 895, -297, -710, -602, 106, -835, 0, 0, 0},
		 .actions[27].servoAngles = {0, 445, 726, 642, -72, 895, -244, -850, -602, 106, -835, 0, 0, 0},
		 .actions[28].servoAngles = {0, 445, 752, 642, -72, 895, -213, -884, -604, 106, -835, 0, 0, 0},
		 .actions[29].servoAngles = {0, 446, 826, 642, -72, 895, -218, -819, -594, 106, -833, 0, 0, 0},
		 .actions[30].servoAngles = {0, 446, 846, 642, -75, 895, -236, -589, -594, 106, -829, 0, 0, 0},
		 .actions[31].servoAngles = {0, 445, 839, 642, -73, 895, -380, -454, -594, 106, -829, 0, 0, 0},
		 .actions[32].servoAngles = {0, 428, 836, 641, -73, 895, -516, -456, -603, 107, -829, 0, 0, 0},
		 .actions[33].servoAngles = {0, 308, 835, 604, -73, 895, -570, -562, -604, 108, -829, 0, 0, 0},
		 .actions[34].servoAngles = {0, 307, 835, 542, -72, 894, -545, -758, -603, 106, -829, 0, 0, 0},
		 .emotionType = EMOTION_NEUTRAL,
		 .total_step = 35,
		 .totalDuration = 5000},
	}};

Motion_t _Active_SittoEat = {
	.point_total = 1,
	.motion = {
		{.actionId = 6, // 单个动作的id
		 .ifNeedBezier = 0,
		 .actions[0].servoAngles = {0, 97, 858, 557, -53, 897, -41, -810, -600, 94, -828, 0, 0, 0},
		 .actions[1].servoAngles = {0, 97, 858, 557, -53, 897, -41, -810, -600, 94, -828, 0, 0, 0},
		 .actions[2].servoAngles = {0, 97, 859, 557, -53, 897, -41, -810, -600, 94, -828, 0, 0, 0},
		 .actions[3].servoAngles = {0, 97, 858, 552, -53, 897, -41, -811, -600, 94, -828, 0, 0, 0},
		 .actions[4].servoAngles = {0, 97, 859, 552, -53, 897, -40, -811, -600, 93, -828, 0, 0, 0},
		 .actions[5].servoAngles = {0, 94, 858, 551, -47, 896, -35, -809, -600, 83, -828, 0, 0, 0},
		 .actions[6].servoAngles = {0, 94, 857, 551, 22, 896, 65, -808, -600, 20, -828, 0, 0, 0},
		 .actions[7].servoAngles = {0, 50, 856, 550, 53, 896, 74, -808, -600, -23, -832, 0, 0, 0},
		 .actions[8].servoAngles = {0, 34, 856, 550, 45, 896, 72, -808, -600, -18, -831, 0, 0, 0},
		 .actions[9].servoAngles = {0, 72, 856, 550, 44, 899, 70, -808, -600, -17, -831, 0, 0, 0},
		 .actions[10].servoAngles = {0, 234, 856, 550, 44, 899, -108, -808, -600, -17, -831, 0, 0, 0},
		 .actions[11].servoAngles = {0, 380, 856, 550, 44, 899, -339, -808, -600, -17, -831, 0, 0, 0},
		 .actions[12].servoAngles = {0, 401, 857, 550, 44, 899, -380, -817, -600, -17, -831, 0, 0, 0},
		 .actions[13].servoAngles = {0, 374, 857, 550, 70, 896, -379, -817, -599, -37, -829, 0, 0, 0},
		 .actions[14].servoAngles = {0, 266, 857, 550, 103, 899, -289, -818, -599, -57, -828, 0, 0, 0},
		 .actions[15].servoAngles = {0, 180, 857, 550, 104, 899, -76, -809, -599, -57, -828, 0, 0, 0},
		 .actions[16].servoAngles = {0, 149, 858, 550, 94, 899, -33, -809, -599, -53, -832, 0, 0, 0},
		 .actions[17].servoAngles = {0, 275, 857, 550, 94, 899, -225, -815, -599, -48, -832, 0, 0, 0},
		 .actions[18].servoAngles = {0, 519, 857, 550, 94, 899, -404, -818, -599, -48, -832, 0, 0, 0},
		 .actions[19].servoAngles = {0, 554, 857, 550, 105, 900, -504, -819, -599, -59, -830, 0, 0, 0},
		 .actions[20].servoAngles = {0, 551, 858, 550, 106, 899, -434, -820, -599, -81, -832, 0, 0, 0},
		 .actions[21].servoAngles = {0, 405, 868, 550, 106, 898, -254, -820, -599, -97, -832, 0, 0, 0},
		 .actions[22].servoAngles = {0, 195, 867, 550, 125, 900, -75, -819, -599, -97, -830, 0, 0, 0},
		 .actions[23].servoAngles = {0, 155, 867, 555, 133, 901, -12, -810, -599, -92, -828, 0, 0, 0},
		 .actions[24].servoAngles = {0, 180, 865, 556, 133, 901, -167, -815, -599, -90, -828, 0, 0, 0},
		 .actions[25].servoAngles = {0, 332, 866, 552, 133, 901, -258, -819, -599, -90, -828, 0, 0, 0},
		 .actions[26].servoAngles = {0, 466, 867, 555, 133, 901, -397, -820, -605, -90, -828, 0, 0, 0},
		 .actions[27].servoAngles = {0, 465, 868, 552, 133, 901, -479, -820, -601, -90, -828, 0, 0, 0},
		 .actions[28].servoAngles = {0, 465, 868, 550, 145, 902, -458, -820, -599, -115, -828, 0, 0, 0},
		 .actions[29].servoAngles = {0, 395, 868, 550, 177, 899, -327, -820, -599, -169, -829, 0, 0, 0},
		 .actions[30].servoAngles = {0, 260, 868, 550, 220, 902, -231, -819, -599, -175, -828, 0, 0, 0},
		 .actions[31].servoAngles = {0, 244, 867, 550, 222, 901, -125, -819, -599, -176, -829, 0, 0, 0},
		 .actions[32].servoAngles = {0, 244, 867, 555, 223, 900, -97, -819, -599, -176, -832, 0, 0, 0},
		 .actions[33].servoAngles = {0, 254, 867, 556, 215, 900, -111, -819, -600, -175, -833, 0, 0, 0},
		 .actions[34].servoAngles = {0, 295, 859, 551, 213, 900, -324, -819, -600, -167, -831, 0, 0, 0},
		 .emotionType = EMOTION_NEUTRAL,
		 .total_step = 35,
		 .totalDuration = 5000},
	}};

Motion_t _Action_Hug = {
	.point_total = 2,
	.motion = {
		{
			.actionId = 7, // 单个动作的id
		 .ifNeedBezier = 1,
		 .total_step = 20,
		 .emotionType = EMOTION_NEUTRAL,
		 .totalDuration = 2000,
		 .endservoAngles = {0, -3, 779, 607, -62, 1056, 87, -845, -513, 96, -1097, 0, 0, 0}},
		{
			.actionId = 8, // 单个动作的id
		 .ifNeedBezier = 1,
		 .total_step = 20,
		 .emotionType = EMOTION_NEUTRAL,
		 .totalDuration = 2000,
		 .endservoAngles = {0, -2, 453, 326, -62, 1056, 87, -277, -360, 96, -1097, 0, 0, 0}}}};

//趴着躺
Motion_t _Action_LieDown = {
	.point_total = 1,
	.motion = {
		{.actionId = 9, // 单个动作的id
		 .ifNeedBezier = 1,
		 .emotionType = EMOTION_NEUTRAL,
		 .total_step = 35,
		 .totalDuration = 5000,
		 .endservoAngles = {0, 662, 778, 432, -843, 1044, -720, -678, -564, 867, -1096, 0, 0, 0}}
	}};

Motion_t _Action_Sit2Prone;
Motion_t _Action_PronetoSit;
Motion_t _Action_Lie2Standup;
Motion_t _Action_Crawl;
Motion_t _Action_Crawlrepeat;
Motion_t _Action_Hello;

// 下面为实际动作序列的定义
>>>>>>> Stashed changes
ServoActionSeries Action_Hello;

/*-------------拥抱---------*/
ServoActionSeries Action_Hug = (ServoActionSeries){
	.actionId = 2,
	.ifNeedBezier = 1,
	.total_step = 20,
	.emotionType = EMOTION_NEUTRAL,
	.totalDuration = 2000,
	.endservoAngles = {0, -3, 779, 607, -62, 1056, 87, -845, -513, 96, -1097, 0, 0, 0}};
//-2, 453, 326, 87, -277, -360
// 0
ServoActionSeries Action_1_Hug = (ServoActionSeries){
	.actionId = 16,
	.ifNeedBezier = 1,
	.total_step = 20,
	.emotionType = EMOTION_NEUTRAL,
	.totalDuration = 2000,
	.endservoAngles = {0, -2, 453, 326, -62, 1056, 87, -277, -360, 96, -1097, 0, 0, 0}};

ServoActionSeries Action_BIGLIE = (ServoActionSeries){
	.actionId = 15,
	.ifNeedBezier = 0,
	.actions[0] = (ServoActionStep){
		.servoAngles = {0, -10, -402, 71, -114, 16, -3, 206, -27, -19, -242, 0, 0, 0},
		.stepDuration = 2},
	.emotionType = EMOTION_NEUTRAL,
	.total_step = 1,
	.totalDuration = 5000}; // 大字躺

/*-------------站起来---------*/

ServoActionSeries Action_1_Standup = (ServoActionSeries){
	.actionId = 13,
	.ifNeedBezier = 1,
	.total_step = 35,
	.emotionType = EMOTION_NEUTRAL,
	.totalDuration = 2000,
	.endservoAngles = {0, 20, -406, 325, -543, 629, -73, 539, -442, 651, -629, 0, 0, 0}};

ServoActionSeries Action_Standup = (ServoActionSeries){
	.actionId = 3,
	.ifNeedBezier = 1,
	.total_step = 35,
	.emotionType = EMOTION_NEUTRAL,
	.totalDuration = 2000,
	.endservoAngles = {0, 19, 420, 318, -302, 866, -59, -421, -350, 347, -841, 0, 0, 0}};

/*-------------坐起来---------*/
ServoActionSeries Action_1_Sit2Prone = (ServoActionSeries){
	.actionId = 14,
	.ifNeedBezier = 1,
	.total_step = 40,
	.emotionType = EMOTION_NEUTRAL,
	.totalDuration = 2000,
	.endservoAngles = {0, 219, 867, 472, 551, 616, -238, -754, -611, -578, -628, 0, 0, 0}};
// 跪着
ServoActionSeries Action_Sit2Prone = (ServoActionSeries){
	.actionId = 4,
	.ifNeedBezier = 1,
	.total_step = 25,
	.emotionType = EMOTION_NEUTRAL,
	.totalDuration = 2000,
	.endservoAngles = {0, -400, 621, 529, -62, 1056, 400, -657, -462, 96, -1097, 0, 0, 0}}; // 坐
// 坐起来

ServoActionSeries Action_PronetoSit;
ServoActionSeries Action_Lie2Standup;
ServoActionSeries Action_Crawl;
ServoActionSeries Action_Crawlrepeat;

ServoActionSeries Action_Walk = (ServoActionSeries){
	.actionId = 1,
<<<<<<< Updated upstream
	.ifNeedBezier = 0,
	.actions[0] = (ServoActionStep){
		.servoAngles = {0, 97, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0},
		.stepDuration = 2},
	.actions[1] = (ServoActionStep){.servoAngles = {0, 97, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0}, .stepDuration = 2},
	.actions[2] = (ServoActionStep){.servoAngles = {0, 300, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0}, .stepDuration = 2},
	.actions[3] = (ServoActionStep){.servoAngles = {0, 266, 442, 410, 42, 796, -81, -647, -527, 218, -1082, 0, 0, 0}, .stepDuration = 2},
	.actions[4] = (ServoActionStep){.servoAngles = {0, 179, 535, 542, 16, 914, -87, -591, -527, 198, -946, 0, 0, 0}, .stepDuration = 2},
	.actions[5] = (ServoActionStep){.servoAngles = {0, 84, 614, 644, -123, 890, 100, -539, -700, 250, -915, 0, 0, 0}, .stepDuration = 2},
	.actions[6] = (ServoActionStep){.servoAngles = {0, 84, 614, 644, -123, 890, 100, -539, -700, 125, -755, 0, 0, 0}, .stepDuration = 2},
	.actions[7] = (ServoActionStep){.servoAngles = {0, 7, 563, 647, -188, 843, 96, -603, -704, -61, -850, 0, 0, 0}, .stepDuration = 2},
	.actions[8] = (ServoActionStep){.servoAngles = {0, 13, 644, 651, -243, 1100, 38, -600, -692, 78, -823, 0, 0, 0}, .stepDuration = 2},
	.actions[9] = (ServoActionStep){.servoAngles = {0, 13, 644, 651, -243, 1050, -275, -598, -692, 78, -823, 0, 0, 0}, .stepDuration = 2},
	.actions[10] = (ServoActionStep){.servoAngles = {0, 13, 644, 651, -243, 1050, -250, -639, -315, 78, -823, 0, 0, 0}, .stepDuration = 2},
	.actions[11] = (ServoActionStep){.servoAngles = {0, 19, 580, 658, -325, 900, -125, -614, -600, 250, -1050, 0, 0, 0}, .stepDuration = 2},
	.actions[12] = (ServoActionStep){.servoAngles = {0, 19, 580, 658, -200, 700, -125, -614, -600, 250, -1050, 0, 0, 0}, .stepDuration = 2},
	.actions[13] = (ServoActionStep){.servoAngles = {0, 24, 586, 662, -22, 860, 27, -639, -674, 349, -1053, 0, 0, 0}, .stepDuration = 2},
	.actions[14] = (ServoActionStep){.servoAngles = {0, 97, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0}, .stepDuration = 2},

	.emotionType = EMOTION_NEUTRAL,
	.total_step = 15,
	.totalDuration = 5000

};
ServoActionSeries Action_TEACH;
ServoActionSeries Action_Test = (ServoActionSeries){
	.actionId = 9,
=======

	.actions[0].servoAngles = {0, 97, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0},
	.actions[1].servoAngles = {0, 97, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0},
	.actions[2].servoAngles = {0, 300, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0},
	.actions[3].servoAngles = {0, 266, 442, 410, 42, 796, -81, -647, -527, 218, -1082, 0, 0, 0},
	.actions[4].servoAngles = {0, 179, 535, 542, 16, 914, -87, -591, -527, 198, -946, 0, 0, 0},
	.actions[5].servoAngles = {0, 84, 614, 644, -123, 890, 100, -539, -700, 250, -915, 0, 0, 0},
	.actions[6].servoAngles = {0, 84, 614, 644, -123, 890, 100, -539, -700, 125, -755, 0, 0, 0},
	.actions[7].servoAngles = {0, 7, 563, 647, -188, 843, 96, -603, -704, -61, -850, 0, 0, 0},
	.actions[8].servoAngles = {0, 13, 644, 651, -243, 1100, 38, -600, -692, 78, -823, 0, 0, 0},
	.actions[9].servoAngles = {0, 13, 644, 651, -243, 1050, -275, -598, -692, 78, -823, 0, 0, 0},
	.actions[10].servoAngles = {0, 13, 644, 651, -243, 1050, -250, -639, -315, 78, -823, 0, 0, 0},
	.actions[11].servoAngles = {0, 19, 580, 658, -325, 900, -125, -614, -600, 250, -1050, 0, 0, 0},
	.actions[12].servoAngles = {0, 19, 580, 658, -200, 700, -125, -614, -600, 250, -1050, 0, 0, 0},
	.actions[13].servoAngles = {0, 24, 586, 662, -22, 860, 27, -639, -674, 349, -1053, 0, 0, 0},
	.actions[14].servoAngles = {0, 97, 504, 658, 37, 801, -75, -642, -524, 217, -1082, 0, 0, 0},

	.emotionType = EMOTION_NEUTRAL,
	.total_step = 15,
	.totalDuration = 5000};

ServoActionSeries Action_TEACH;
ServoActionSeries Action_Test = (ServoActionSeries){
>>>>>>> Stashed changes

	.actions[0] = (ServoActionStep){
		.servoAngles = {672, -5, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0},
		.stepDuration = 2},
	.actions[1] = (ServoActionStep){.servoAngles = {672, -5, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[2] = (ServoActionStep){.servoAngles = {672, -5, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[3] = (ServoActionStep){.servoAngles = {672, -5, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[4] = (ServoActionStep){.servoAngles = {672, -3, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[5] = (ServoActionStep){.servoAngles = {655, -6, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[6] = (ServoActionStep){.servoAngles = {410, -6, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[7] = (ServoActionStep){.servoAngles = {744, -1, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[8] = (ServoActionStep){.servoAngles = {1010, -6, 365, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[9] = (ServoActionStep){.servoAngles = {693, -32, 365, -37, 134, 130, 555, -118, -478, -903, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[10] = (ServoActionStep){.servoAngles = {377, -32, 365, -37, 134, 130, 555, -118, -478, -903, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[11] = (ServoActionStep){.servoAngles = {739, -79, 365, -37, 134, 130, 555, -118, -478, -903, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[12] = (ServoActionStep){.servoAngles = {1008, -84, 365, -37, 134, 130, 555, -118, -478, -903, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[13] = (ServoActionStep){.servoAngles = {605, -79, 367, -37, 134, 130, 555, -118, -478, -903, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[14] = (ServoActionStep){.servoAngles = {405, -47, 366, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[15] = (ServoActionStep){.servoAngles = {746, -122, 367, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[16] = (ServoActionStep){.servoAngles = {978, -107, 368, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[17] = (ServoActionStep){.servoAngles = {636, -71, 367, -37, 134, 130, 555, -118, -478, -904, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[18] = (ServoActionStep){.servoAngles = {428, -61, 367, -37, 134, 130, 555, -118, -478, -904, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[19] = (ServoActionStep){.servoAngles = {723, -144, 367, -37, 134, 130, 555, -118, -478, -904, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[20] = (ServoActionStep){.servoAngles = {826, -151, 368, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[21] = (ServoActionStep){.servoAngles = {450, -138, 366, -37, 134, 130, 555, -118, -478, -903, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[22] = (ServoActionStep){.servoAngles = {381, -117, 366, -37, 134, 130, 555, -118, -478, -904, -99, -183, 0, 0}, .stepDuration = 2},
	.actions[23] = (ServoActionStep){.servoAngles = {724, -138, 366, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[24] = (ServoActionStep){.servoAngles = {742, -137, 366, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[25] = (ServoActionStep){.servoAngles = {347, -110, 366, -37, 134, 130, 555, -118, -478, -903, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[26] = (ServoActionStep){.servoAngles = {589, -102, 366, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[27] = (ServoActionStep){.servoAngles = {899, -105, 366, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[28] = (ServoActionStep){.servoAngles = {691, -113, 366, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[29] = (ServoActionStep){.servoAngles = {411, -95, 366, -37, 134, 130, 555, -118, -478, -903, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[30] = (ServoActionStep){.servoAngles = {543, -84, 365, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[31] = (ServoActionStep){.servoAngles = {863, -85, 365, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[32] = (ServoActionStep){.servoAngles = {923, -85, 365, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[33] = (ServoActionStep){.servoAngles = {678, -87, 365, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0}, .stepDuration = 2},
	.actions[34] = (ServoActionStep){.servoAngles = {551, -85, 365, -37, 134, 130, 555, -118, -478, -904, -98, -183, 0, 0}, .stepDuration = 2},

<<<<<<< Updated upstream
=======
ServoActionSeries Active_Wave = (ServoActionSeries){
	.actionId = 3,

	.actions[0].servoAngles = {0, 169, 613, 559, -49, 896, -266, -540, -552, 90, -827, 0, 0, 0},
	.actions[1].servoAngles = {0, 169, 613, 559, -49, 896, -266, -540, -552, 90, -827, 0, 0, 0},
	.actions[2].servoAngles = {0, 169, 614, 559, -49, 896, -266, -540, -552, 90, -827, 0, 0, 0},
	.actions[3].servoAngles = {0, 169, 615, 559, -49, 898, -266, -547, -553, 89, -827, 0, 0, 0},
	.actions[4].servoAngles = {0, 169, 615, 559, -49, 896, -266, -548, -553, 89, -827, 0, 0, 0},
	.actions[5].servoAngles = {0, 169, 615, 559, -49, 896, -276, -541, -548, 89, -827, 0, 0, 0},
	.actions[6].servoAngles = {0, 168, 612, 559, -59, 896, -291, -540, -551, 94, -827, 0, 0, 0},
	.actions[7].servoAngles = {0, 168, 611, 557, -60, 896, -508, -543, -553, 95, -827, 0, 0, 0},
	.actions[8].servoAngles = {0, 168, 614, 559, -50, 896, -777, -541, -553, 91, -827, 0, 0, 0},
	.actions[9].servoAngles = {0, 175, 615, 561, -48, 897, -766, -540, -553, 88, -827, 0, 0, 0},
	.actions[10].servoAngles = {0, 176, 616, 561, -48, 897, -523, -541, -553, 88, -827, 0, 0, 0},
	.actions[11].servoAngles = {0, 176, 616, 561, -48, 897, -226, -547, -553, 88, -827, 0, 0, 0},
	.actions[12].servoAngles = {0, 176, 616, 561, -49, 897, -8, -549, -553, 88, -827, 0, 0, 0},
	.actions[13].servoAngles = {0, 169, 615, 561, -59, 897, -42, -549, -553, 94, -827, 0, 0, 0},
	.actions[14].servoAngles = {0, 169, 608, 560, -60, 897, -310, -549, -554, 95, -827, 0, 0, 0},
	.actions[15].servoAngles = {0, 169, 614, 560, -58, 897, -628, -549, -554, 95, -827, 0, 0, 0},
	.actions[16].servoAngles = {0, 175, 616, 561, -49, 898, -759, -549, -554, 90, -827, 0, 0, 0},
	.actions[17].servoAngles = {0, 176, 616, 561, -48, 898, -630, -548, -554, 88, -827, 0, 0, 0},
	.actions[18].servoAngles = {0, 176, 616, 561, -48, 898, -349, -548, -554, 88, -827, 0, 0, 0},
	.actions[19].servoAngles = {0, 176, 616, 561, -49, 899, -85, -549, -554, 88, -827, 0, 0, 0},
	.actions[20].servoAngles = {0, 170, 616, 561, -49, 899, -13, -550, -554, 89, -827, 0, 0, 0},
	.actions[21].servoAngles = {0, 169, 615, 561, -59, 899, -169, -549, -554, 94, -827, 0, 0, 0},
	.actions[22].servoAngles = {0, 169, 615, 561, -59, 899, -470, -541, -554, 94, -827, 0, 0, 0},
	.actions[23].servoAngles = {0, 169, 615, 561, -50, 899, -736, -550, -554, 94, -827, 0, 0, 0},
	.actions[24].servoAngles = {0, 169, 616, 555, -55, 899, -785, -581, -564, 95, -827, 0, 0, 0},
	.actions[25].servoAngles = {0, 175, 616, 555, -49, 898, -783, -765, -598, 90, -827, 0, 0, 0},
	.actions[26].servoAngles = {0, 176, 616, 561, -48, 898, -710, -800, -597, 89, -827, 0, 0, 0},
	.actions[27].servoAngles = {0, 176, 616, 561, -48, 898, -464, -799, -596, 89, -827, 0, 0, 0},
	.actions[28].servoAngles = {0, 176, 616, 561, -49, 897, -242, -798, -596, 89, -827, 0, 0, 0},
	.actions[29].servoAngles = {0, 176, 616, 561, -49, 897, -117, -798, -596, 89, -827, 0, 0, 0},
	.actions[30].servoAngles = {0, 172, 615, 561, -50, 897, -106, -799, -597, 91, -827, 0, 0, 0},
	.actions[31].servoAngles = {0, 171, 615, 561, -50, 897, -106, -799, -597, 90, -827, 0, 0, 0},
	.actions[32].servoAngles = {0, 170, 615, 561, -50, 897, -106, -799, -597, 90, -827, 0, 0, 0},
	.actions[33].servoAngles = {0, 169, 615, 561, -50, 897, -106, -799, -597, 90, -827, 0, 0, 0},
	.actions[34].servoAngles = {0, 169, 608, 561, -49, 897, -106, -799, -597, 90, -827, 0, 0, 0},

>>>>>>> Stashed changes
	.emotionType = EMOTION_NEUTRAL,
	.total_step = 35,
	.totalDuration = 5000

};

<<<<<<< Updated upstream
ServoActionSeries Active_Micromove_1 = (ServoActionSeries){

};

ServoActionSeries Active_Micromove_2 = (ServoActionSeries){

};

ServoActionSeries Active_Micromove_3 = (ServoActionSeries){
=======
ServoActionSeries Active_Sit = (ServoActionSeries){
	.actionId = 4,
	.actions[0].servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -827, 0, 0, 0},
	.actions[1].servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -827, 0, 0, 0},
	.actions[2].servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -827, 0, 0, 0},
	.actions[3].servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -827, 0, 0, 0},
	.actions[4].servoAngles = {0, 113, 892, 644, 388, 903, -124, -798, -595, -361, -828, 0, 0, 0},
	.actions[5].servoAngles = {0, 117, 892, 644, 388, 903, -132, -798, -595, -361, -835, 0, 0, 0},
	.actions[6].servoAngles = {0, 121, 892, 644, 376, 904, -134, -797, -595, -349, -838, 0, 0, 0},
	.actions[7].servoAngles = {0, 122, 891, 644, 303, 904, -135, -797, -595, -310, -838, 0, 0, 0},
	.actions[8].servoAngles = {0, 122, 891, 644, 285, 904, -135, -797, -595, -247, -837, 0, 0, 0},
	.actions[9].servoAngles = {0, 122, 891, 644, 234, 904, -135, -797, -595, -189, -838, 0, 0, 0},
	.actions[10].servoAngles = {0, 122, 891, 644, 100, 904, -135, -803, -595, -78, -830, 0, 0, 0},
	.actions[11].servoAngles = {0, 121, 891, 644, -55, 904, -135, -803, -595, 89, -828, 0, 0, 0},
	.actions[12].servoAngles = {0, 121, 891, 644, -73, 904, -135, -804, -595, 105, -828, 0, 0, 0},
	.actions[13].servoAngles = {0, 121, 891, 644, -72, 903, -134, -802, -595, 105, -828, 0, 0, 0},
	.actions[14].servoAngles = {0, 120, 891, 644, -72, 903, -134, -801, -595, 107, -827, 0, 0, 0},
	.actions[15].servoAngles = {0, 116, 887, 642, -72, 899, -133, -797, -595, 107, -827, 0, 0, 0},
	.actions[16].servoAngles = {0, 123, 709, 642, -72, 895, -136, -796, -599, 110, -828, 0, 0, 0},
	.actions[17].servoAngles = {0, 123, 486, 642, -72, 895, -136, -796, -600, 106, -836, 0, 0, 0},
	.actions[18].servoAngles = {0, 171, 422, 642, -72, 895, -134, -803, -596, 105, -837, 0, 0, 0},
	.actions[19].servoAngles = {0, 188, 425, 643, -72, 896, -133, -806, -596, 106, -831, 0, 0, 0},
	.actions[20].servoAngles = {0, 279, 423, 643, -72, 895, -136, -795, -594, 106, -836, 0, 0, 0},
	.actions[21].servoAngles = {0, 445, 424, 643, -72, 895, -168, -795, -594, 106, -836, 0, 0, 0},
	.actions[22].servoAngles = {0, 447, 425, 643, -72, 895, -356, -724, -593, 106, -836, 0, 0, 0},
	.actions[23].servoAngles = {0, 447, 426, 643, -72, 895, -461, -670, -593, 106, -836, 0, 0, 0},
	.actions[24].servoAngles = {0, 447, 427, 643, -72, 895, -456, -674, -595, 106, -836, 0, 0, 0},
	.actions[25].servoAngles = {0, 446, 481, 642, -72, 895, -436, -674, -595, 106, -836, 0, 0, 0},
	.actions[26].servoAngles = {0, 446, 653, 642, -72, 895, -297, -710, -602, 106, -835, 0, 0, 0},
	.actions[27].servoAngles = {0, 445, 726, 642, -72, 895, -244, -850, -602, 106, -835, 0, 0, 0},
	.actions[28].servoAngles = {0, 445, 752, 642, -72, 895, -213, -884, -604, 106, -835, 0, 0, 0},
	.actions[29].servoAngles = {0, 446, 826, 642, -72, 895, -218, -819, -594, 106, -833, 0, 0, 0},
	.actions[30].servoAngles = {0, 446, 846, 642, -75, 895, -236, -589, -594, 106, -829, 0, 0, 0},
	.actions[31].servoAngles = {0, 445, 839, 642, -73, 895, -380, -454, -594, 106, -829, 0, 0, 0},
	.actions[32].servoAngles = {0, 428, 836, 641, -73, 895, -516, -456, -603, 107, -829, 0, 0, 0},
	.actions[33].servoAngles = {0, 308, 835, 604, -73, 895, -570, -562, -604, 108, -829, 0, 0, 0},
	.actions[34].servoAngles = {0, 307, 835, 542, -72, 894, -545, -758, -603, 106, -829, 0, 0, 0},

	.emotionType = EMOTION_NEUTRAL,
	.total_step = 35,
	.totalDuration = 5000

};

ServoActionSeries Active_SittoEat = (ServoActionSeries){
	.actionId = 5,

	.actions[0].servoAngles = {0, 97, 858, 557, -53, 897, -41, -810, -600, 94, -828, 0, 0, 0},
	.actions[1].servoAngles = {0, 97, 858, 557, -53, 897, -41, -810, -600, 94, -828, 0, 0, 0},
	.actions[2].servoAngles = {0, 97, 859, 557, -53, 897, -41, -810, -600, 94, -828, 0, 0, 0},
	.actions[3].servoAngles = {0, 97, 858, 552, -53, 897, -41, -811, -600, 94, -828, 0, 0, 0},
	.actions[4].servoAngles = {0, 97, 859, 552, -53, 897, -40, -811, -600, 93, -828, 0, 0, 0},
	.actions[5].servoAngles = {0, 94, 858, 551, -47, 896, -35, -809, -600, 83, -828, 0, 0, 0},
	.actions[6].servoAngles = {0, 94, 857, 551, 22, 896, 65, -808, -600, 20, -828, 0, 0, 0},
	.actions[7].servoAngles = {0, 50, 856, 550, 53, 896, 74, -808, -600, -23, -832, 0, 0, 0},
	.actions[8].servoAngles = {0, 34, 856, 550, 45, 896, 72, -808, -600, -18, -831, 0, 0, 0},
	.actions[9].servoAngles = {0, 72, 856, 550, 44, 899, 70, -808, -600, -17, -831, 0, 0, 0},
	.actions[10].servoAngles = {0, 234, 856, 550, 44, 899, -108, -808, -600, -17, -831, 0, 0, 0},
	.actions[11].servoAngles = {0, 380, 856, 550, 44, 899, -339, -808, -600, -17, -831, 0, 0, 0},
	.actions[12].servoAngles = {0, 401, 857, 550, 44, 899, -380, -817, -600, -17, -831, 0, 0, 0},
	.actions[13].servoAngles = {0, 374, 857, 550, 70, 896, -379, -817, -599, -37, -829, 0, 0, 0},
	.actions[14].servoAngles = {0, 266, 857, 550, 103, 899, -289, -818, -599, -57, -828, 0, 0, 0},
	.actions[15].servoAngles = {0, 180, 857, 550, 104, 899, -76, -809, -599, -57, -828, 0, 0, 0},
	.actions[16].servoAngles = {0, 149, 858, 550, 94, 899, -33, -809, -599, -53, -832, 0, 0, 0},
	.actions[17].servoAngles = {0, 275, 857, 550, 94, 899, -225, -815, -599, -48, -832, 0, 0, 0},
	.actions[18].servoAngles = {0, 519, 857, 550, 94, 899, -404, -818, -599, -48, -832, 0, 0, 0},
	.actions[19].servoAngles = {0, 554, 857, 550, 105, 900, -504, -819, -599, -59, -830, 0, 0, 0},
	.actions[20].servoAngles = {0, 551, 858, 550, 106, 899, -434, -820, -599, -81, -832, 0, 0, 0},
	.actions[21].servoAngles = {0, 405, 868, 550, 106, 898, -254, -820, -599, -97, -832, 0, 0, 0},
	.actions[22].servoAngles = {0, 195, 867, 550, 125, 900, -75, -819, -599, -97, -830, 0, 0, 0},
	.actions[23].servoAngles = {0, 155, 867, 555, 133, 901, -12, -810, -599, -92, -828, 0, 0, 0},
	.actions[24].servoAngles = {0, 180, 865, 556, 133, 901, -167, -815, -599, -90, -828, 0, 0, 0},
	.actions[25].servoAngles = {0, 332, 866, 552, 133, 901, -258, -819, -599, -90, -828, 0, 0, 0},
	.actions[26].servoAngles = {0, 466, 867, 555, 133, 901, -397, -820, -605, -90, -828, 0, 0, 0},
	.actions[27].servoAngles = {0, 465, 868, 552, 133, 901, -479, -820, -601, -90, -828, 0, 0, 0},
	.actions[28].servoAngles = {0, 465, 868, 550, 145, 902, -458, -820, -599, -115, -828, 0, 0, 0},
	.actions[29].servoAngles = {0, 395, 868, 550, 177, 899, -327, -820, -599, -169, -829, 0, 0, 0},
	.actions[30].servoAngles = {0, 260, 868, 550, 220, 902, -231, -819, -599, -175, -828, 0, 0, 0},
	.actions[31].servoAngles = {0, 244, 867, 550, 222, 901, -125, -819, -599, -176, -829, 0, 0, 0},
	.actions[32].servoAngles = {0, 244, 867, 555, 223, 900, -97, -819, -599, -176, -832, 0, 0, 0},
	.actions[33].servoAngles = {0, 254, 867, 556, 215, 900, -111, -819, -600, -175, -833, 0, 0, 0},
	.actions[34].servoAngles = {0, 295, 859, 551, 213, 900, -324, -819, -600, -167, -831, 0, 0, 0},

	.emotionType = EMOTION_NEUTRAL,
	.total_step = 35,
	.totalDuration = 5000
>>>>>>> Stashed changes

};

ServoActionSeries Active_Micromove_4 = (ServoActionSeries){

};

ServoActionSeries Active_Micromove_5 = (ServoActionSeries){

};

ServoActionSeries Active_Micromove_6 = (ServoActionSeries){

};

ServoActionSeries Active_Micromove_7 = (ServoActionSeries){

};
// 开启示教模式
void Action_Teachmode_Init(void)
{
	TEACHMODE = 1;
}
// 示教结束后向上位机发送的代码格式（可设置）
void Action_Teachmode(void)
{
	for (int si = 0; si < 35; si++)
	{
<<<<<<< Updated upstream
		printf("	.actions[%d] = (ServoActionStep){", si);
		printf("\r\n");
=======
		printf("	.actions[%d]", si);
>>>>>>> Stashed changes
		printf(".servoAngles = {");
		for (int i = 0; i < 13; i++)
		{
			printf("%d, ", Action_TEACH.actions[si].servoAngles[i]);
		}
		printf("%d ", Action_TEACH.actions[si].servoAngles[13]);
		printf("},\r\n	.stepDuration = 2\r\n");
		printf("},\r\n");
	}
	printf("	.emotionType = EMOTION_NEUTRAL,\r\n		.total_step = 35,\r\n	.totalDuration = 5000\r\n");
}
// 动作库初始化函数
void Action_init(void)
{
	Action_index[0] = &Action_TEACH;
	Action_index[1] = &Action_Walk;
	Action_index[2] = &Action_Hug;
	Action_index[3] = &Action_Standup;
	Action_index[4] = &Action_Sit2Prone;
	Action_index[5] = &Action_PronetoSit;
	Action_index[6] = &Action_Lie2Standup;
	Action_index[7] = &Action_Crawl;
<<<<<<< Updated upstream
	Action_index[8] = &Action_Crawlrepeat;
	Action_index[9] = &Action_Test;
	Action_index[10] = &Action_Unit1;
	Action_index[11] = &Action_UnitTest;
	Action_index[12] = &Action_LieDown;
	Action_index[13] = &Action_1_Standup;
	Action_index[14] = &Action_1_Sit2Prone;
	Action_index[15] = &Action_BIGLIE;
	Action_index[16] = &Action_1_Hug;
=======
	Action_index[10] = &Action_Crawlrepeat;
	Action_index[14] = &Action_Test;
>>>>>>> Stashed changes

	Action_TEACH.actionId = 0;
	Action_TEACH.emotionType = EMOTION_NEUTRAL;
	Action_TEACH.totalDuration = 5000;
	Action_TEACH.total_step = TEACH_TOTAL_STEP;
<<<<<<< Updated upstream

	/*
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
	//1：挥手动作
	Action_Hello.actionId = 1;
	Action_Hello.emotionType = EMOTION_HAPPY;
	Action_Hello.totalDuration = 5000;
	Action_Hello.total_step = 6;

	Action_Hello.actions[0] = (ServoActionStep){
		.servoAngles = {1650, 200, 0, 0, 900, 0, 900, 0, 0, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	Action_Hello.actions[1] = (ServoActionStep){
		.servoAngles = {1650, 0, -500, 0, 900, 0, 900, 0, 0, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	Action_Hello.actions[2] = (ServoActionStep){
		.servoAngles = {1650, 200, 0, 0, 900, 0, 900, 0, 0, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	Action_Hello.actions[3] = (ServoActionStep){
		.servoAngles = {950, 200, -300, 0, 900, 0, 900, 0, 0, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	Action_Hello.actions[4] = (ServoActionStep){
		.servoAngles = {1650, 200, -300, 0, 900, 0, 900, 0, 0, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	Action_Hello.actions[5] = (ServoActionStep){
		.servoAngles = {700, 200, 0, 0, 900, 0, 900, 0, 0, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};

	//2：拥抱动作
	Action_Hug.actionId = 2;
	Action_Hug.emotionType = EMOTION_HAPPY;
	Action_Hug.totalDuration = 5000;
	Action_Hug.total_step = 4;
	Action_Hug.actions[0] = (ServoActionStep){
		.servoAngles = {1000, 200, -300, 0, 900, 0, 900, 0, 0, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	Action_Hug.actions[1] = (ServoActionStep){
		.servoAngles = {1000, 500, 100, 0, 900, 0, 900, -500, -100, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	Action_Hug.actions[2] = (ServoActionStep){
		.servoAngles = {1000, -200, -300, 0, 900, 0, 900, 200, 300, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	Action_Hug.actions[3] = (ServoActionStep){
		.servoAngles = {700, 200, -300, 0, 900, 0, 900, 0, 0, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};

	//3：全趴姿势到站立
	Action_Standup.actionId = 3;
	Action_Standup.emotionType = EMOTION_HAPPY;
	Action_Standup.totalDuration = 5000;
	Action_Standup.total_step = 2;
	Action_Standup.actions[0] = (ServoActionStep){
		.servoAngles = {950, -700, 0, 0, -700, 0, 900, 700, 0, -900, 700, 0, 0, 0},
		.stepDuration = 2
	};
	Action_Standup.actions[1] = (ServoActionStep){
		.servoAngles = {950, 0, 0, 0, 0, 0, 900, 0, 0, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};

	//4：坐姿到向前趴着
	Action_Sit2Prone.actionId = 4;
	Action_Sit2Prone.emotionType = EMOTION_HAPPY;
	Action_Sit2Prone.totalDuration = 5000;
	Action_Sit2Prone.total_step = 2;
	Action_Sit2Prone.totalDuration = 5000;
	Action_Sit2Prone.total_step = 2;
	Action_Sit2Prone.totalDuration = 5000;
	Action_Sit2Prone.total_step = 2;
	Action_Sit2Prone.actions[0] = (ServoActionStep){
		.servoAngles = {950, 0, 0, 0, 0, 0, 900, 0, 0, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	Action_Sit2Prone.actions[1] = (ServoActionStep){
		.servoAngles = {1100, 0, 0, -350, 0, 0, 750, 0, 0, -550, 0, 0, 0, 0},
		.stepDuration = 2
	};

	//5：由向前趴转为坐姿
	Action_PronetoSit.actionId = 5;
	Action_PronetoSit.emotionType = EMOTION_HAPPY;
	Action_PronetoSit.total_step = 2;
	Action_PronetoSit.totalDuration = 5000;
	Action_PronetoSit.actions[0] = (ServoActionStep){
		.servoAngles = {1100, 0, 0, -350, 0, 0, 750, 0, 0, -550, 0, 0, 0, 0},
		.stepDuration = 2
	};
	Action_PronetoSit.actions[1] = (ServoActionStep){
		.servoAngles = {1400, 0, 0, 0, 0, 0, 400, 0, 0, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};

	//6：由趴转为站姿
	Action_Lie2Standup.actionId = 6;
	Action_Lie2Standup.emotionType = EMOTION_HAPPY;
	Action_Lie2Standup.totalDuration = 5000;
	Action_Lie2Standup.total_step = 2;
	Action_Lie2Standup.actions[0] = (ServoActionStep){
		.servoAngles = {350, -200, 750, -550, -200, 750, 1450, 200, -750, -450, 200, -750, 0, 0},
		.stepDuration = 2
	};
	Action_Lie2Standup.actions[1] = (ServoActionStep){
		.servoAngles = {950, -200, 550, 0, -200, 550, 900, 200, -550, -900, 200, -550, 0, 0},
		.stepDuration = 2
	};

	//7：由趴转为站姿
	Action_Crawl.actionId = 7;
	Action_Crawl.emotionType = EMOTION_HAPPY;
	Action_Crawl.totalDuration = 5000;
	Action_Crawl.total_step = 20;
	Action_Crawl.actions[0] = (ServoActionStep){
		.servoAngles = {900, -300, 700, 100, 0, 0, 900, 300, -700, -1000, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//右腿抬腿（起步）
	Action_Crawl.actions[1] = (ServoActionStep){
		.servoAngles = {500, -300, 700, 100, 0, 0, 900, 300, -700, -1000, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左前腿往后++右前腿落下
	Action_Crawl.actions[2] = (ServoActionStep){
		.servoAngles = {700, -300, 500, 100, 0, 0, 700, 300, -700, -1000, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左后腿向前&右后腿向后
	Action_Crawl.actions[3] = (ServoActionStep){
		.servoAngles = {700, -300, 500, 200, 0, 0, 700, 300, -700, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左前臂收手
	Action_Crawl.actions[4] = (ServoActionStep){
		.servoAngles = {700, -300, 500, 200, 0, 0, 700, 300, -900, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左前臂向前，右前臂向后一部分
	Action_Crawl.actions[5] = (ServoActionStep){
		.servoAngles = {900, -300, 700, 200, 0, 0, 1300, 300, -900, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左右前臂落地
	Action_Crawl.actions[6] = (ServoActionStep){
		.servoAngles = {1100, -300, 700, 200, 0, 0, 1100, 300, -500, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左右后腿推动前行
	Action_Crawl.actions[7] = (ServoActionStep){
		.servoAngles = {1100, -300, 700, 0, 0, 0, 1100, 300, -500, -1100, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//右前臂收手
	Action_Crawl.actions[8] = (ServoActionStep){
		.servoAngles = {1100, -300, 900, 0, 0, 0, 1100, 300, -500, -1100, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//右前臂向前，左前臂向后一部分
	Action_Crawl.actions[9] = (ServoActionStep){
		.servoAngles = {500, -300, 900, 0, 0, 0, 900, 300, -700, -1100, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左右前臂落地
	Action_Crawl.actions[10] = (ServoActionStep){
		.servoAngles = {700, -300, 500, 0, 0, 0, 700, 300, -700, -1100, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左后腿向前&右后腿向后
	Action_Crawl.actions[11] = (ServoActionStep){
		.servoAngles = {700, -300, 500, 200, 0, 0, 700, 300, -700, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左前臂收手
	Action_Crawl.actions[12] = (ServoActionStep){
		.servoAngles = {700, -300, 500, 200, 0, 0, 700, 300, -900, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左前臂向前，右前臂向后一部分
	Action_Crawl.actions[13] = (ServoActionStep){
		.servoAngles = {900, -300, 700, 200, 0, 0, 1300, 300, -700, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左右前臂落地
	Action_Crawl.actions[14] = (ServoActionStep){
		.servoAngles = {1100, -300, 700, 200, 0, 0, 1100, 300, -500, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左右后腿推动前行
	Action_Crawl.actions[15] = (ServoActionStep){
		.servoAngles = {1100, -300, 700, 0, 0, 0, 1100, 300, -500, -1100, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//右前臂收手
	Action_Crawl.actions[16] = (ServoActionStep){
		.servoAngles = {1100, -300, 900, 0, 0, 0, 1100, 300, -500, -1100, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//右前臂向前，左前臂向后一部分
	Action_Crawl.actions[17] = (ServoActionStep){
		.servoAngles = {500, -300, 900, 0, 0, 0, 900, 300, -700, -1100, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左右前臂落地
	Action_Crawl.actions[18] = (ServoActionStep){
		.servoAngles = {700, -300, 500, 0, 0, 0, 700, 300, -700, -1100, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左后腿向前&右后腿向后
	Action_Crawl.actions[19] = (ServoActionStep){
		.servoAngles = {700, -300, 500, 200, 0, 0, 700, 300, -700, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};

	//7：由趴转为站姿
	Action_Crawlrepeat.actionId = 10;
	Action_Crawlrepeat.emotionType = EMOTION_HAPPY;
	Action_Crawlrepeat.totalDuration = 5000;
	Action_Crawlrepeat.total_step = 8;
	//左前臂收手
	Action_Crawlrepeat.actions[0] = (ServoActionStep){
		.servoAngles = {700, -300, 500, 200, 0, 0, 700, 300, -900, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左前臂向前，右前臂向后一部分
	Action_Crawlrepeat.actions[1] = (ServoActionStep){
		.servoAngles = {900, -300, 700, 200, 0, 0, 1300, 300, -900, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左右前臂落地
	Action_Crawlrepeat.actions[2] = (ServoActionStep){
		.servoAngles = {1100, -300, 700, 200, 0, 0, 1100, 300, -500, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左右后腿推动前行
	Action_Crawlrepeat.actions[3] = (ServoActionStep){
		.servoAngles = {1100, -300, 700, 0, 0, 0, 1100, 300, -500, -1100, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//右前臂收手
	Action_Crawlrepeat.actions[4] = (ServoActionStep){
		.servoAngles = {1100, -300, 900, 0, 0, 0, 1100, 300, -500, -1100, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//右前臂向前，左前臂向后一部分
	Action_Crawlrepeat.actions[5] = (ServoActionStep){
		.servoAngles = {500, -300, 900, 0, 0, 0, 900, 300, -700, -1100, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左右前臂落地
	Action_Crawlrepeat.actions[6] = (ServoActionStep){
		.servoAngles = {700, -300, 500, 0, 0, 0, 700, 300, -700, -1100, 0, 0, 0, 0},
		.stepDuration = 2
	};
	//左后腿向前&右后腿向后
	Action_Crawlrepeat.actions[7] = (ServoActionStep){
		.servoAngles = {700, -300, 500, 200, 0, 0, 700, 300, -700, -900, 0, 0, 0, 0},
		.stepDuration = 2
	};*/

	// 6：由趴转为站姿
	/*Action_Lie2Standup.actionId = 7;
	Action_Lie2Standup.emotionType = EMOTION_HAPPY;
	Action_Lie2Standup.totalDuration = 5000;
	Action_Lie2Standup.total_step = 2;
	Action_Lie2Standup.actions[0] = (ServoActionStep){
		.discretePoints = {
			{0.1, 0.2, 0.3},                {0.7, 0.8, 0.9},
			{0.4, 0.5, 0.6},                {0.7, 0.8, 0.9},
			{1.0, 1.1, 1.2},                {1.3, 1.4, 1.5},
			{0.1, 0.2, 0.3},                {0.1, 0.2, 0.3},
			{0.1, 0.2, 0.3},                {0.1, 0.2, 0.3},
			{0.1, 0.2, 0.3},                {0.1, 0.2, 0.3},
			{0.1, 0.2, 0.3},                {0.1, 0.2, 0.3}
		},
		.servoAngles = {350, -200, 750, -550, -200, 750, 1450, 200, -750, -450, 200, -750, 0, 0},
		.stepDuration = 2
	};
	Action_Lie2Standup.actions[1] = (ServoActionStep){
		.discretePoints = {
			{0.1, 0.2, 0.3},                {0.7, 0.8, 0.9},
			{0.4, 0.5, 0.6},                {0.7, 0.8, 0.9},
			{1.0, 1.1, 1.2},                {1.3, 1.4, 1.5},
			{0.1, 0.2, 0.3},                {0.1, 0.2, 0.3},
			{0.1, 0.2, 0.3},                {0.1, 0.2, 0.3},
			{0.1, 0.2, 0.3},                {0.1, 0.2, 0.3},
			{0.1, 0.2, 0.3},                {0.1, 0.2, 0.3}
		},
		.servoAngles = {950, -200, 550, 0, -200, 550, 900, 200, -550, -900, 200, -550, 0, 0},
		.stepDuration = 2
	};*/
=======
>>>>>>> Stashed changes
}
