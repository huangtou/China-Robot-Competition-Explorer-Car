#include "sys.h"

//TIM9  168MHZ     PSC 336-1      ARR 10000-1
//标准舵机的脉宽范围是 0.5ms ~ 2.5ms，对应角度 0°~180°：
//  │    参数    │     标准舵机     │
//
//  │ 0° 脉宽   │ 0.5ms (CCR=250)  │
// 
//  │ 90° 脉宽  │ 1.5ms (CCR=750)  │
// 
//  │ 180° 脉宽 │ 2.5ms (CCR=1250) │
// 根据实际值

//设置舵机
//身体起立1->200，趴0->560
//摄像头平行于舵机1->200,仰视0->620
void DJ(int angle1,int angle2,int angle3,int angle4)//身体,摄像头,右手,左手
{
	int angle_3=240;
	int angle_4=240;
	if (angle1==0) TIM9->CCR2=500;
	else if (angle1==1) TIM9->CCR2=200;
	if (angle2==0) TIM9->CCR1=560;
	else if (angle2==1) TIM9->CCR1=200;

	angle_3=(u16)(5.56f*angle3+240);
	angle_4=(u16)(5.56f*angle4+240);
	TIM12->CCR1=angle_3;
	TIM12->CCR2=angle_4;
	
//	TIM_SetCompare1(TIM9,angle4);//头
}

//上电舵机初始化，起立双手放下
//从后往前看
//身体：1起
//摄像头：1平行//与身体的数字保持一致即可
//右：0下
//左：180下
void Dj_all_Init(void)
{
	DJ(1,1,0,180);
}

//开始舵机准备，起立双手举起
//从后往前看
//身体：1起
////摄像头：1平行//与身体的数字保持一致即可
//右：180上
//左：0上
void DJ_ready(void)
{
	DJ(1,1,180,0);
}

//舵机起，起立双手放下
void DJ_over(void)
{
	DJ(1,1,0,180);
}

//舵机趴，趴下双手放下
void DJ_down(void)
{
	DJ(0,0,0,180);
}

void DJ_QQB(void)
{
	int angle_3=240;
	int angle_4=240;
	TIM9->CCR2=350;

	angle_3=(u16)(5.56f*0+240);
	angle_4=(u16)(5.56f*180+240);
	TIM12->CCR1=angle_3;
	TIM12->CCR2=angle_4;
}

//舵机起，左手举起，甩右手
void DJ_pt(void)
{
	DJ(1,1,0,0);
	move_control(0,0,0,0);
	HAL_Delay(500);
	DJ(1,1,180,0);
}

void DJ_sp(void)
{
	DJ(0,1,0,180);
}

void DJ_camera(void)
{
	TIM9->CCR2=500;
	TIM9->CCR1=650;
}
void DJ_camera_look(void)
{
	TIM9->CCR2=200;
	TIM9->CCR1=170;
}
void DJ_camera_looklook(void)
{
	TIM9->CCR2=250;
	TIM9->CCR1=250;
}
