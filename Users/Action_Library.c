#include "Action_library.h"
#include "user_servo.h"
ServoActionSeries* Action_index[50];
ServoActionSeries Action_Hello;
ServoActionSeries Action_Hug;
ServoActionSeries Action_Standup;
ServoActionSeries Action_Sit2Prone;
ServoActionSeries Action_PronetoSit;
ServoActionSeries Action_Lie2Standup;
ServoActionSeries Action_Crawl;
ServoActionSeries Action_Crawlrepeat;

void Action_init(void)
{
	Action_index[1] = &Action_Hello;
	Action_index[2] = &Action_Hug;
	Action_index[3] = &Action_Standup;
	Action_index[4] = &Action_Sit2Prone;
	Action_index[5] = &Action_PronetoSit;
	Action_index[6] = &Action_Lie2Standup;
	Action_index[7] = &Action_Crawl;
	Action_index[10] = &Action_Crawlrepeat;
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
    };
	
	
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



