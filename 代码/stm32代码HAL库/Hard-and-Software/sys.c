#include "sys.h"

int ADW[30]={ 
                      3200/*0*/,3200/*1*/,3200/*2*/,3200/*3*/,3200/*4*/,3200/*5*/,3200/*6*/,3200/*7*/,3200/*8*/,3200/*9*/,
                     
                     3200/*10*/,3200/*11*/,3200/*12*/,3200/*13*/,1000/*14*/,1000/*15*/,1000/*16*/,500/*17*/,1500/*18*/,1050/*19*/,
                     
                      1500/*20*/,1900/*21*/,1150/*22*/,1200/*23*/,1000/*24*/,200/*25*/,200/*26*/,500/*27*/,200/*28*/,400/*29*/
};

void Para_Init(void)
{
	//速度内环
	PID_Init(&pid_1,0.8,0.6,0.22);//0.29//给1号轮子写一套PID的结构体，指针形式
	PID_Init(&pid_2,0.8,0.6,0.22);
	PID_Init(&pid_3,0.8,0.6,0.22);
	PID_Init(&pid_4,0.8,0.6,0.22);//0.8,0.6,0.22//0.7,0.6,0.99
	
	//
	PID_Init(&pid_A,2.7,0,2.3);//角度PID初始化//8.10：2.0,0,2
	PID_Init(&pid_R,0.04,0.5,0.05);//转角度PID初始化0.01,0.03,0有点慢0.085,0.1
	PID_Init(&pid_F,0.008,0.01,0.0);//位置

////////////////灰度外环
	//(0,85]
	car.huidu_P1   =0.14f;
	car.huidu_D1   =0.15f;
	//(85,110]
	car.huidu_P2   =0.11f;     //8.10：0.07//10.22:0.14//10.25:0.11f(交通灯左右巡不太好)//lyx到此一游
	car.huidu_D2   =0.16f;     //8.10：0.1 //10.22:0.15//10.25:0.16f
	//(110,180]
	car.huidu_P3   =0.055f;     //8.10：0.018//10.22:0.055f//10.25:0.031f
	car.huidu_D3   =0.09f;     //8.10：0.1  //10.22:0.09f //10.25:0.083f
	//(180,280)
	car.huidu_P4   =0.0085f;   //8.10：0.0018//10.23 250:0.0085f
	car.huidu_D4   =0.05f;     //8.10：0.05  //10.23 250:0.05f
	//[280,300+)
	car.huidu_P5   =0.009f;		 //10.23_1:0.0082f//10.23_2 280:0.009f  //10.23_3 300:0.009f
	car.huidu_D5   =0.04f;	   //10.23_1:0.06f  //10.23_2 280:0.0447f //10.23_3 300:0.04f可以再小一点
	
////////////////CCD外环
	car.CCD_P1=0.05f;//0.08f
	car.CCD_D1=0.01f;//0.09f

	car.line_11=15.0f;
	car.line_l=15.0f;
	car.line_n=15.0f;
	
////////////////

	car.Av  =5.5f;		//小写v//灰度中值		//car结构体内部的变量，在sys.h 188中统一声明了全局变量
	car.bili=0.4f;												//car结构体内部的变量，在sys.h 188中统一声明了全局变量
	car.AV  =15.0f;		//大写V							//car结构体内部的变量，在sys.h 188中统一声明了全局变量
	car.AV2 =15.0f;												//car结构体内部的变量，在sys.h 188中统一声明了全局变量
	
	car.minav  = 50;
	car.minav2 = 50;
	
	
	//自适应曝光参数
	car.auto_enable=1;//默认开启自动曝光
	car.auto_min = 1;//delay_time最小
	car.auto_max = 5;//delay_time最大
	car.auto_av  =17;//±3内不管
	car.delay_time=19;
	
	
}

void Display(void)
{
	if (__HAL_TIM_GET_FLAG(&htim7, TIM_FLAG_UPDATE) != RESET)  {	__HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);	}
	HAL_TIM_Base_Stop_IT(&htim7);
	CHANNEL_CCD=ADC_CHANNEL_5;
	printf("t13.txt=\"Frow\"\xFF\xFF\xFF");				//"编号.类型="文本"结束符"
	while(1)
	{
		if (KEY_Scan(0)== Key_Mid_Press)
		{
			if (CHANNEL_CCD==ADC_CHANNEL_5)
			{
				printf("t13.txt=\"Frow\"\xFF\xFF\xFF");	//"编号.类型="文本"结束符"
				CHANNEL_CCD=ADC_CHANNEL_5;
			}
			else 
			{
				printf("t13.txt=\"Back\"\xFF\xFF\xFF");	//"编号.类型="文本"结束符"
				CHANNEL_CCD=ADC_CHANNEL_4;
			}
		}
		Read_128(CHANNEL_CCD);
		cou++;
		if (cou>50)//CCD
		{
			cou=0;
			printf("cle s0.id,0\xff\xff\xff");				//清楚指令 位置，行号 结束符
			printf("addt s0.id,0,128\xff\xff\xff");		//显示指令 位置，起始行，数据个数 结束符
			HAL_Delay(2);
			for(int i =127;i>=0;i--)
			{
				printf("%c",Pixel[i]);
			}
			printf("\x01\xff\xff\xff");
		}
		 Gap_Avr(Pixel, 4);
		 Aver128(Pixel,0,31);
     	 auto_Exposure();
		 HAL_Delay(car.delay_time);

			HSL_Read_Sensor(HSL_value);
			printf("H0.val=%d\xFF\xFF\xFF",HSL_value[0]);	//"位置编号.显示内容\xFF\xFF\xFF",数值//HSL_value[0]
			printf("n0.val=%d\xFF\xFF\xFF",AD_value[0]);
			printf("n1.val=%d\xFF\xFF\xFF",AD_value[1]);
			printf("n2.val=%d\xFF\xFF\xFF",AD_value[2]);
			printf("n3.val=%d\xFF\xFF\xFF",AD_value[3]);
			printf("n4.val=%d\xFF\xFF\xFF",AD_value[4]);
			printf("n5.val=%d\xFF\xFF\xFF",AD_value[5]);
			printf("n6.val=%d\xFF\xFF\xFF",AD_value[6]);
			printf("n7.val=%d\xFF\xFF\xFF",AD_value[7]);
			printf("n8.val=%d\xFF\xFF\xFF",AD_value[8]);
			printf("n9.val=%d\xFF\xFF\xFF",AD_value[9]);
			printf("n10.val=%d\xFF\xFF\xFF",AD_value[10]);
			printf("n11.val=%d\xFF\xFF\xFF",AD_value[11]);
			ReadHWT101();
			printf("TL0.val=%d\xFF\xFF\xFF",(int)car.Angle);
			printf("G1.val=%d\xFF\xFF\xFF",GD(1));	//GD(1)//usart2_dataRcvd[0]
			printf("G2.val=%d\xFF\xFF\xFF",GD(2));	//GD(2)//usart2_dataRcvd[1]
			printf("G5.val=%d\xFF\xFF\xFF",GD(5));	//GD(5)//usart2_ZLflag
			printf("G6.val=%d\xFF\xFF\xFF",GD(6));
			printf("G7.val=%d\xFF\xFF\xFF",GD(7));
			printf("G8.val=%d\xFF\xFF\xFF",GD(8));
			printf("J1.val=%d\xFF\xFF\xFF",GD(11));
			printf("J2.val=%d\xFF\xFF\xFF",GD(12));
			printf("Q1.val=%d\xFF\xFF\xFF",GD(9));
	}

}















void Delay_us(uint32_t nus)
{
 uint32_t Delay = nus * 168/4;
 do
 {
  __NOP();
 }
 while (Delay --);
}

void Delay_ms(uint32_t nms)
{
 nms*=100;
 do
 {
  Delay_us(10);
 }
 while (nms --);
}


