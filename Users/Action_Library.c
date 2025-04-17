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


ServoActionSeries Active_Micromove_1 = (ServoActionSeries){
		
};

ServoActionSeries Active_Micromove_2 = (ServoActionSeries){
		
};

ServoActionSeries Active_Micromove_3 = (ServoActionSeries){
		
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
	Action_index[3] = &Action_Standup;
	Action_index[4] = &Action_Sit2Prone;
	Action_index[5] = &Action_PronetoSit;
	Action_index[6] = &Action_Lie2Standup;
	Action_index[7] = &Action_Crawl;
	Action_index[10] = &Action_Crawlrepeat;
	Action_index[14] = &Action_Test;
	
	Action_TEACH.actionId = 0;
	Action_TEACH.emotionType = EMOTION_NEUTRAL;
	Action_TEACH.totalDuration = 5000;
	Action_TEACH.total_step = TEACH_TOTAL_STEP;
	
	
	
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
	
	
	//6：由趴转为站姿
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
}



