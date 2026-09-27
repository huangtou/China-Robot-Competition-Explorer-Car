#ifndef Kalman_H
#define Kalman_H


typedef struct 
{
    float Last_P;//上次估算协方差 不可以为0 ! ! ! ! ! 
    float Now_P;//当前估算协方差
    float out1;	//卡尔曼滤波器输出
	float out2;	//卡尔曼滤波器输出
	float out3;	//卡尔曼滤波器输出
	float out4;	//卡尔曼滤波器输出
    float Kg;//卡尔曼增益
    float Q;//过程噪声协方差
    float R;//观测噪声协方差
}Kalman;

void Kalman_Init(void);
float KalmanFilter1(Kalman *kfp,float input);
float KalmanFilter2(Kalman *kfp,float input);
float KalmanFilter3(Kalman *kfp,float input);
float KalmanFilter4(Kalman *kfp,float input);
extern Kalman kfp;

#endif
