#include "sys.h"

void Kalman_Init()
{
	kfp.Last_P = 0.001;			
	kfp.Now_P = 0;		
	kfp.out1 = 0;
	kfp.out2 = 0;			
	kfp.out3 = 0;				
	kfp.out4 = 0;				
	kfp.Kg = 0;		
	kfp.Q = 0.0005;//测量值
	kfp.R = 0.025;//预测值
}

/**
 *卡尔曼滤波器
 *@param 	Kalman *kfp 卡尔曼结构体参数
 *   			float input 需要滤波的参数的测量值（即传感器的采集值）
 *@return 滤波后的参数（最优值）
 */
float KalmanFilter1(Kalman *kfp,float input)
{
   //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
   kfp->Now_P = kfp->Last_P + kfp->Q;
   //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
   kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
   //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
   kfp->out1 = kfp->out1 + kfp->Kg * (input -kfp->out1);//因为这一次的预测值就是上一次的输出值
   //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
   kfp->Last_P = (1-kfp->Kg) * kfp->Now_P;
   return kfp->out1;
}
float KalmanFilter2(Kalman *kfp,float input)
{
   //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
   kfp->Now_P = kfp->Last_P + kfp->Q;
   //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
   kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
   //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
   kfp->out2 = kfp->out2 + kfp->Kg * (input -kfp->out2);//因为这一次的预测值就是上一次的输出值
   //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
   kfp->Last_P = (1-kfp->Kg) * kfp->Now_P;
   return kfp->out2;
}
float KalmanFilter3(Kalman *kfp,float input)
{
   //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
   kfp->Now_P = kfp->Last_P + kfp->Q;
   //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
   kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
   //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
   kfp->out3 = kfp->out3 + kfp->Kg * (input -kfp->out3);//因为这一次的预测值就是上一次的输出值
   //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
   kfp->Last_P = (1-kfp->Kg) * kfp->Now_P;
   return kfp->out3;
}float KalmanFilter4(Kalman *kfp,float input)
{
   //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
   kfp->Now_P = kfp->Last_P + kfp->Q;
   //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
   kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
   //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
   kfp->out4 = kfp->out4 + kfp->Kg * (input -kfp->out4);//因为这一次的预测值就是上一次的输出值
   //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
   kfp->Last_P = (1-kfp->Kg) * kfp->Now_P;
   return kfp->out4;
}
