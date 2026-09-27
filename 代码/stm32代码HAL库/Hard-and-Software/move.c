#include "sys.h"

extern volatile int P_V_limit;

extern volatile int A_limit;

float mabs(float a)		//函数功能：绝对值计算
{
	if(a<=0) return -a;
 	else  return a;
}

void limit(float* integral,float limitation)	//函数功能：积分限幅
{
	if(*integral>=mabs(limitation)) *integral=mabs(limitation);
 	else if(*integral<=-mabs(limitation)) *integral=-mabs(limitation);
}

void move_init(void)	//函数功能：小车移动初始化
{
	//开启编码器定时器（读取编码器信号、计算电机转速/位置）
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);	
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
	//开启许多PWM（输出PWM波，驱动执行器）
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);				//使能2号电机PWM
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);				//使能2号电机PWM（正转）
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);				//使能1号电机PWM（正转）
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);				//使能1号电机PWM

	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);				//使能4号电机PWM
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);				//使能4号电机PWM（正转）
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);				//使能3号电机PWM（正转）
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);				//使能3号电机PWM
	
	HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);				
	HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_2);

}


void move_control(int wheel_1,int wheel_2,int wheel_3,int wheel_4)	//函数功能：控制电机正反转
{
	for(uint8_t i=0;i<4;i++)
	{
		switch(i)
		{
			case 0: if(wheel_1>=900)wheel_1=900;else if(wheel_1<-900)wheel_1=-900;	//从后往前看：1号电机 对应 左前方轮子
					if(wheel_1>=0) {TIM4->CCR3=wheel_1;  TIM4->CCR4=0;        break;}
					else		   {TIM4->CCR3=0;        TIM4->CCR4=-wheel_1; break;}
			case 1:if(wheel_2>=900)wheel_2=900;else if(wheel_2<-900)wheel_2=-900;		//从后往前看：2号电机 对应 右前方轮子
					if(wheel_2>=0) {TIM4->CCR2=wheel_2;  TIM4->CCR1=0;        break;}
					else		   {TIM4->CCR2=0;        TIM4->CCR1=-wheel_2; break;}
			case 2: if(wheel_3>=900)wheel_3=900;else if(wheel_3<-900)wheel_3=-900;	//从后往前看：3号电机 对应 左后方轮子
					if(wheel_3>=0) {TIM5->CCR3=wheel_3;  TIM5->CCR4=0;break;}
					else		   {TIM5->CCR3=0;        TIM5->CCR4=-wheel_3; break;}
			case 3: if(wheel_4>=900)wheel_4=900;else if(wheel_4<-900)wheel_4=-900;	//从后往前看：4号电机 对应 右后方轮子
					if(wheel_4>=0) {TIM5->CCR2=wheel_4;  TIM5->CCR1=0;        break;}
					else		   {TIM5->CCR2=0;        TIM5->CCR1=-wheel_4; break;}
		}
	}
}

void move_velocity(void)
{
	velocity[1]=(short)TIM1->CNT;
	TIM1->CNT=0;
	velocity[2]=-(short)TIM2->CNT;
	TIM2->CNT=0;
	velocity[3]=(short)TIM8->CNT;
	TIM8->CNT=0;
	velocity[4]=-(short)TIM3->CNT;
	TIM3->CNT=0;
}

void move_encoder(void)	//函数功能：读取编码器每20ms的计数变化量//读取每个编码器的总脉冲数 后参与Timer.c中292行 平均总脉冲数的计算
{
	encoder[1]+=(short)TIM1->CNT;		//累计的是编码器1从启动到当前的总脉冲数//后续通过”总脉冲数 ÷ 分辨率 = 总圈数”以及轮径、减速比等参数可转换为实际移动距离（如 go_enco 函数中的 dis*2700/50 就是脉冲数到距离的转换系数）
	encoder[2]+=-(short)TIM2->CNT;	//编码器安装/接线导致原始计数方向相反，所以要递减
	encoder[3]+=(short)TIM8->CNT;		
	encoder[4]+=-(short)TIM3->CNT;	
}

void PID_Init(volatile PID* pid, float kp, float ki, float kd)	//函数功能：给PID结构体快速赋值
{
	pid->kp=kp;
	pid->ki=ki;
	pid->kd=kd;
	pid->error=0;
	pid->lasterror=0;
	pid->preerror=0;
	pid->integral=0;
	pid->output=0;
}

//PID*pid:PID数据类型		pid指针-地址	填入&pid_A
//target:目标
//actual:实际
void PID_calc_A(volatile PID*pid, float target, float actual)	//函数功能：转角度PID
{
	//计算error并限幅
	pid->error=target-actual;
	if(pid->error>=180) pid->error=pid->error-360;
	else if(pid->error<=-180) pid->error=pid->error+360;
	//计算output并限幅
	if(pid->error<=1.0f&&pid->error>=-1.0f) pid->output=0;
	else 
	{
		pid->output=pid->kp*(pid->error)+pid->kd*(pid->error-pid->lasterror);
		pid->lasterror=pid->error;
	}
	limit((float*)&pid->output,A_limit);
}

void PID_calc_V(volatile PID*pid, float target, float actual)	//函数功能：调速PID，计算output//PID结构体指针，目标速度，实际速度
{
	pid->error=target-actual;
	
	pid->output+=pid->kp*(pid->error-pid->lasterror)+pid->ki*(pid->error)+pid->kd*(pid->error-2*pid->lasterror+pid->preerror);//增量式PID
	
	pid->preerror=pid->lasterror;
	pid->lasterror=pid->error;
	limit((float*)&pid->output,P_V_limit);											//积分，限幅
}

void PID_change(int speed)	//函数功能：根据不同的速度，自动适配PID
{
	if(speed<0)
		speed=-speed;
	if(speed<=85)		//专门左右巡线的PID
	{
		car.huidu_P = car.huidu_P1;
		car.huidu_D = car.huidu_D1;
	}
	if(speed>85 && speed<=110)	
	{
		car.huidu_P = car.huidu_P2;
		car.huidu_D =car.huidu_D2;				
	}
	else if(speed>110 && speed<=180)	
	{
		car.huidu_P = car.huidu_P3;
		car.huidu_D =car.huidu_D3;				
	}
	else if(speed>180 && speed<280)
	{
		car.huidu_P = car.huidu_P4;
		car.huidu_D = car.huidu_D4;		
	}
	else if(speed>=280)
	{
		car.huidu_P = car.huidu_P5;
		car.huidu_D = car.huidu_D5;		
	}
}

void huidu_ccd_pid(void)	//灰度巡线，CCD巡线的PID
{
	if(car.Direct==0)//向前走,用灰度循线
	{
		car.ADC_value = Huidu_findline(AD_data,car.Line_Search);//灰度找线函数（这个函数的功能就是把白线位置，即对应的灰度编号找出来）//形参：灰度值，左中右巡，car.ADC_value实际白线位置对应的灰度编号
		if(car.ADC_value>=0&&car.ADC_value<=11.001f)						//如果找到线了
		{
			PID_change(car.Target);																//根据速度改变PID大小的函数//形参：目标速度
			car.Error=car.Av - car.ADC_value;											//误差 = 白线目标位置-白线实际位置
			car.Pout=car.huidu_P*car.Error;												//P
			car.Dout=car.huidu_D*(car.Error-car.Last_Error);			//D
			car.Last_Error=car.Error;
			car.OUT=car.Pout+car.Dout;														//算输出值
			
		//防止超控//可修改
			if(car.OUT>0.8f)
				car.OUT=0.8f;
			else if(car.OUT<-0.8f)
				car.OUT=-0.8f;;
		}
		else
		{
			car.OUT=0;
		}
	}
	else if(car.Direct==-1)//向后走&&即用后面的CCD//与速度赋值无关！！！！
	{
		if(car.ccd_use>2)
			if(car.find!=-1.0)//入果找到了线，再PID
				PIDCCD(car.find,car.CCD_P1,car.CCD_D1);//模式2
	}
	else //向前走&&即用前面的CCD//与速度赋值无关！！！！
	{
		if(car.ccd_use>2)
			if(car.find!=-1)//入果找到了线，再PID
				PIDCCD(car.find,car.CCD_P1,car.CCD_D1);
	}
}

