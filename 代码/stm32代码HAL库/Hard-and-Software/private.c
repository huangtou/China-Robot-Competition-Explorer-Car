#include "sys.h"

int flagroad = 0;//记录交通信号牌
int HDflay;//回调专用标志位
int flag;
uint16_t bobao = 1600;

volatile uint8_t BackOnceFlag = 1;

int my_gd1(void)
{
	if(GD1==1)
	{
	return 0;
	}
else return 1;
}

int my_gd2(void)
{
	if(GD2==1)
	{
	return 0;
	}
else return 1;
}

int my_gd2_enco(void)
{
	car.enco=1;
	if(GD2==1 || (int)(car.Target_dis*2700/50)-car.enco_counter<=1)
	{
		car.Target=0;																						//目标速度（清零）
		car.enco=0;																							//编码器使能标志位（关闭）
		encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;		//每个编码器速度（清零）
		car.enco_counter=0;
		return 0;
	}
else return 1;
}

int my_gd5(void)
{
	if(GD5==1)
	{
	return 0;
	}
else return 1;
}

int my_gd7(void)
{
	if(GD7==1)
	{
	return 0;
	}
else return 1;
}

int my_gd8(void)
{
	if(GD8==1)
	{
	return 0;
	}
else return 1;
}

int my_gd2_gd8(void)
{
	if(GD2==0 || GD8==0)
	{
		return 1;
	}
	else return 0;
}
	

int my_gd56(void)
{
	if(GD5==0&&GD6==0)
	{
	return 1;
	}
	else return 0;
}

int my_gd17(void)
{
	if(GD1==0&&GD7==0)
	{
	return 1;
	}
	else return 0;
}

int my_qc1(void) //微动开关1
{
	if(Qc1==1)
	{
	return 1;
	}
else return 0;
}

int my_jg1(void)
{
	if(jg1==1)
	{
	return 0;
	}
else return 1;
}

int my_jg1_gd2(void)
{
	if(jg1==0|| GD2==0)
	{
	return 1;
	}
else return 0;
}

int my_jg2(void)
{
	if(jg2==1)
	{
	return 0;
	}
else return 1;
}

int my_jg2_gd8(void)
{
	if(jg2==0 || GD8==0)
	{
	return 1;
	}
else return 0;
}
int my_jg2_gd2(void)
{
	if(jg2==0 || GD2==0)
	{
	return 1;
	}
else return 0;
}

int my_jg1_jg2_gd2(void)
{
	if(jg1==0 || jg2==0 || GD2==0)
	{
		return 1;
	}
	else return 0;
}

int my_enco(void)//指定距离
{
	car.enco=1;
	if((int)(car.Target_dis*2700/50)-car.enco_counter<=1)			//car.Target_dis*2700/50将目标距离转化为对应的脉冲数，与编码器读取到的平均脉冲数car.enco_counter比较，如果差值小于1则回调打断函数执行，就实现了go_enco的效果
	{
		car.Target=0;																						//目标速度（清零）
		car.enco=0;																							//编码器使能标志位（关闭）
		encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;		//每个编码器速度（清零）
		car.enco_counter=0;																			//编码器平均速度（清零）
		return 1;
	}
	else
		return 0;
}

int my_enco_huidu(void)//指定距离
{
	car.enco=1;
	if(((int)(car.Target_dis*2700/50)-car.enco_counter<=1) || (AD(1)>3200||AD(2)>3200||AD(3)>3200||AD(4)>3200||AD(5)>3200||AD(6)>3200||AD(7)>3200||AD(8)>3200||AD(9)>3200||AD(10)>3200))
	{
		car.Target=0;																						//目标速度（清零）
		car.enco=0;																							//编码器使能标志位（关闭）
		encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;		//每个编码器速度（清零）
		car.enco_counter=0;																			//编码器平均速度（清零）
		return 1;
	}
	else
		return 0;
}

int my_enco_huidu01(void)//指定距离
{
	car.enco=1;
	if(((int)(car.Target_dis*2700/50)-car.enco_counter<=1) || (AD(0)>3200||AD(1)>3200))
	{
		car.Target=0;																						//目标速度（清零）
		car.enco=0;																							//编码器使能标志位（关闭）
		encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;		//每个编码器速度（清零）
		car.enco_counter=0;																			//编码器平均速度（清零）
		return 1;
	}
	else
		return 0;
}

int my_enco_gd2(void)//指定距离
{
	car.enco=1;
	if((int)(car.Target_dis*2700/50)-car.enco_counter<=1||GD2==0)
	{
		car.Target=0;																						//目标速度（清零）
		car.enco=0;																							//编码器使能标志位（关闭）
		encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;		//每个编码器速度（清零）
		car.enco_counter=0;																			//编码器平均速度（清零）
		return 1;
	}
	else
		return 0;
}

int my_enco_gd8(void)//指定距离
{
	car.enco=1;
	if((int)(car.Target_dis*2700/50)-car.enco_counter<=1||GD8==0)
	{
		car.Target=0;																						//目标速度（清零）
		car.enco=0;																							//编码器使能标志位（关闭）
		encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;		//每个编码器速度（清零）
		car.enco_counter=0;																			//编码器平均速度（清零）
		return 1;
	}
	else
		return 0;
}

int my_enco_gd2_gd8(void)//指定距离
{
	car.enco=1;
	if((int)(car.Target_dis*2700/50)-car.enco_counter<=1 || GD8==0 || GD2==0)
	{
		car.Target=0;																						//目标速度（清零）
		car.enco=0;																							//编码器使能标志位（关闭）
		encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;		//每个编码器速度（清零）
		car.enco_counter=0;																			//编码器平均速度（清零）
		return 1;
	}
	else
		return 0;
}

int my_time(void)//定时
{
	car.time--;
	if(car.time==0)
	{
		return 1;
	}
	else
		return 0;
}

int my_time_ZL(void)//让摄像头好好读取一下数字//停止时间不要超过3秒最好
{
	car.time--;
	if(car.time<0)
	{
		car.time = 0;
	}
	if(car.time==0 || ZLflag)
	{
		return 1;
	}
	else
		return 0;
}

int my_time_gd2(void)//定时
{
	car.time--;
	if(car.time==0 || GD2==0)
	{
		return 1;
	}
	else
		return 0;
}

int my_time_qc1(void)//定时
{
	car.time--;
	if(car.time==0 || Qc1==1)
	{
		return 1;
	}
	else
		return 0;
}

int my_speed_260(void)
{
	if(car.Target<=260)
	{
	return 1;
	}
else return 0;
}

int my_no_huidu(void) 
{
	if(AD(0)<3000&&AD(1)<3000&&AD(2)<3000&&AD(3)<3000&&AD(4)<3000&&AD(5)<3000&&AD(6)<3000&&AD(7)<3000&&AD(8)<3000&&AD(9)<3000&&AD(10)<3000&&AD(11)<3000)
	{
		HDflay++;
		if (HDflay>20) {HDflay=0;  return 1;}
		else return 0;

	}
	else return 0;
}


int my_huidu(void) 
{
	if(AD(1)>3200||AD(2)>3200||AD(3)>3200||AD(4)>3200||AD(5)>3200||AD(6)>3200||AD(7)>3200||AD(8)>3200||AD(9)>3200||AD(10)>3200)
	{
		HDflay++;
		if (HDflay>25) {HDflay=0;  return 1;}
		else return 0;
		
	}
	else return 0;
}

int my_jiaozhun(void)
{
	if(AD(1)>2000||AD(2)>2000||AD(3)>2000||AD(4)>2000||AD(5)>2000||AD(6)>2000||AD(7)>2000||AD(8)>2000||AD(9)>2000||AD(10)>2000)
	{
		HDflay++;
		if (HDflay>25) {HDflay=0;  return 1;}
		else return 0;
		
	}
	else return 0;
}

int my_jiaozhun56(void)
{
	if(AD(5)>2000&&AD(6)>2000)
	{
		HDflay++;
		if (HDflay>10) {HDflay=0;  return 1;}
		else return 0;

	}
	else return 0;
}

int my_huidu01(void) 
{

	if(AD(0)>3200||AD(1)>3200)
	{
		HDflay++;
		if (HDflay>10) {HDflay=0;  return 1;}
		else return 0;

	}
	else return 0;
}

int my_huidu56(void) 
{
	if(AD(5)>3200||AD(6)>3200)
	{
		HDflay++;
		if (HDflay>5) {HDflay=0;  return 1;}
		else return 0;
	}
	else return 0;
}

int my_huidu1011(void) 
{
	if(AD(10)>3200||AD(11)>3200)
	{
		HDflay++;
		if (HDflay>5) {HDflay=0;  return 1;}
		else return 0;
	}
	else return 0;
}

int my_huidu11(void) 
{
	if(AD(11)>3200)
	{
		HDflay++;
		if (HDflay>5) {HDflay=0;  return 1;}
		else return 0;
	}
	else return 0;
}

int my_huidu910(void) 
{
	if(AD(9)>3200||AD(10)>3200)
	{
		HDflay++;
		if (HDflay>5) {HDflay=0;  return 1;}
		else return 0;
	}
	else return 0;
}

int my_huidu01_1011(void)
{
	if((AD(0)>3200&&AD(1)>3200)&&(AD(10)>3200&&AD(11)>3200))
	{
		HDflay++;
		if (HDflay>5) {HDflay=0;  return 1;}
		else return 0;
	}
	else return 0;
}

int my_turn90(void)
{
	if(flag==0)
	{
		if(AD(5)<3200&&AD(6)<3200)
		{
			HDflay++;
			if (HDflay>20) {HDflay=0;  flag=1;  return 0;}
			else return 0;
		}
		else return 0;
	}
	else
	{
		if(AD(5)>3200&&AD(6)>3200)
		{
			HDflay++;
			if (HDflay>5) {HDflay=0;  flag=0;  return 1;}
			else return 0;
		}
		else return 0;
	}
}

void jiaozhun(void)
{
	HWT101_Calibrate();
	HWT101_go(0,70,my_jiaozhun);
	if(AD(5)>2000&&AD(6)>2000)
	{
		car.Target_dis = 150;
		HWT101_go(0,70,my_enco);
	}
	else
	{
		if(AD(0)>2000||AD(1)>2000||AD(2)>2000||AD(3)>2000||AD(4)>2000)
		{
			HWT101_Calibrate();
			HWT101_go_rotate(15,70,my_jiaozhun56);

			HWT101_Calibrate();
			car.Target_dis = 150;
			HWT101_go(0,70,my_enco);
		}
		else if(AD(7)>2000||AD(8)>2000||AD(9)>2000||AD(10)>2000||AD(11)>2000)
		{
			HWT101_Calibrate();
			HWT101_go_rotate(-15,70,my_jiaozhun56);

			HWT101_Calibrate();
			car.Target_dis = 150;
			HWT101_go(0,70,my_enco);
		}
	}
}
/////封装函数//////////////////////////////////////////////////////////
void begin(void)
{
	int state=0;

	while(1)
	{
		if(state==0)
		{
			if(GD(7)==0)
			{
				state=1;
			}
		}
		else if(state==1)
		{
			if(GD(7)==1)
			{
				break;
			}
		}
	}
}

void stop(int time)
{
	car.time = time;
	HWT101_Calibrate();	
	HWT101_go(0,0,my_time);
}

void stop_read(int time)
{
	car.time = time;
	HWT101_Calibrate();	
	HWT101_go(0,0,my_time_ZL);
}

void down_home(void)
{
	DJ_over();
	HWT101_Calibrate();								//标定0角度
	DJ_down();
	car.Target_dis=15;
	HWT101_go(0,50,my_enco);

	go_enco(35,50,0,1,NULL);
}

void to_LBridge(void)
{
	DJ_camera();
	
	go_enco(40,120,0,0,NULL);
	go((1<<0)|(1<<1),150,0,0,NULL);		//到岔路口

	go_enco(30,150,0,0,NULL);
	HWT101_Calibrate();
	go_GD(1,120,0,0,NULL);						//到长桥
	
	stop(50);
}

void up_LBridge(void)//长桥上坡
{
	car.Target_dis=65;
	//HWT101_go(0,70,my_enco);
	HWT101_GD(3,70,0,0,my_enco);
}

void go_LBridge(void)//过长桥
{
	car.Target_dis = 20;
	HWT101_GD(3,150,0,0,my_enco);
	car.Target_dis = 55;
	HWT101_GD(3,180,0,0,my_enco);
	car.Target_dis = 15;
	HWT101_GD(3,120,0,0,my_enco);
}

void down_LBridge(void)//长桥下坡
{
	car.Target_dis = 65;
	HWT101_go(3,50,my_enco);
}

void up_platform(void)
{
	
	go_enco(20,75,0,0,NULL);
	HWT101_Calibrate();	
	go_enco(30,75,0,0,my_no_huidu);		//上坡
	car.Target_dis=30;
	HWT101_go(1,75,my_enco);
//	HWT101_Calibrate();					//标定
	HWT101_go(1,75,my_qc1);			//陀螺仪撞平台
	DJ_over();
//	car.time=50;
//	HWT101_go(0,75,my_time);
}

void turn_180(void)
{
	HWT101_go_rotate(-90,45,NULL);
	HAL_Delay(20);
	HWT101_Calibrate();
	HWT101_go_rotate(-90,45,NULL);			//角度--------------
}

void down_platform(void)
{
	HWT101_Calibrate();
	DJ_down();

	car.Target_dis = 15;
	HWT101_go(0,50,my_enco);//下平台
	go_enco(50,50,0,0,NULL);
	
//	jiaozhun();
}

void ting_wen(void)//停稳
{
	go_enco(17,50,0,0,NULL);		//停稳
}
void ting_wen_d(void)//倒车停稳
{
	go_enco(10,-50,-1,0,NULL);		//停稳
}
void qi_bu(void)//起步
{
	car.Target_dis = 13;
	go_acc(100,40,90,0,0,my_enco);
}
void qi_bu_d(void)//倒车起步
{
	go_enco(30,-120,-1,0,NULL);		//起步
}
void HWT101_enco(int enco,int speed)//陀螺仪走一段距离（封装了一下）
{
	HWT101_Calibrate();
	car.Target_dis = enco;
	HWT101_go(0,speed,my_enco);
}
///////////////////////////////////////////////////////////////////

/////跑图
void Ready(void)
{
	begin();
	DJ_ready();
	SYN_FrameInfo(0, (uint8_t *)"[v13][t5]准备完毕");
	stop(500);
}

void part1(void)
{
	LBridge();
	LBto2();
	to2toZL();
	toZLto4();
}

void LBridge(void)//过长桥
{
	//出家门
	down_home();
	
	//去长桥
	to_LBridge();
	
	//上长桥
	up_LBridge();
	
	//过长桥
	go_LBridge();
	
	//下长桥
	down_LBridge();
}

void LBto2(void)
{
	//去2号平台
	go_enco(20,130,0,0,NULL);
	go_enco(50,180,0,0,NULL);
	go_GD(1,120,0,0,NULL);
	HWT101_Calibrate();
	
	//上2号平台
	go_enco(30,70,0,0,NULL);
	car.Target_dis = 30;
	HWT101_go(1,75,my_enco);
	
	//在2号平台
	HWT101_go(1,75,my_qc1);
	DJ_over();
	
	SYN_FrameInfo(0, (uint8_t *)"[v13][t5]到达二号平台");
	
	DJ_pt();
	stop(200);
	turn_180();
	DJ_down();
	DJ_camera();

	//下2号平台
	HWT101_Calibrate();
	car.Target_dis = 15;
	HWT101_go(1,60,my_enco);
	go_enco(50,60,0,0,NULL);
	go_enco(20,150,0,0,NULL);
}

void to2toZL(void)
{
	//进入岔路口
	car.Target_dis = 60;
	//go_acc(50,110,90,0,1,my_enco);
	go_enco(60,110,0,1,my_huidu1011);
	HWT101_Calibrate();
	HWT101_go_rotate(-30,60,NULL);
	go_GD(1,120,0,1,NULL);
	
	//过阶梯
	go_enco(80,90,0,0,NULL);
	go_enco(35,90,0,0,NULL);
	
	go_enco(120,90,0,0,NULL);
	
	//进入直线
	go_acc(100,150,280,0,0,my_huidu1011);
	
	go_enco(50,300,0,0,NULL);
	go_acc(100,300,110,0,-1,my_huidu1011);
	
	car.Target_dis = 10;
	go_acc(50,110,100,0,-1,my_enco);
	
	//去往直立景点
	go_enco(50,85,0,-1,NULL);																																																																	//lyx
	DJ_down();
	car.Target_dis = 50;
	go_acc(100,150,250,0,0,my_enco);
	DJ_ready();
	car.Target_dis = 60;
	go_acc(50,250,150,0,0,my_enco);
	
	//到直立
	
	go_enco(40,80,0,0,my_qc1);
	go_enco(5,40,0,0,my_qc1);
	stop(50);
		
	//倒车准备识别
	go_enco(15,-80,-1,0,NULL);
	//开启识别
	if(!ZL[0])
	{
		
		USART2_StartRead();
		
	}
	//记录
	car.Direct = 0;	//前面有倒车一定要加，不然语音播报不响
	if(!ZL[0] || ZLflag)
	{
		//stop_read(500);
		stop_read(TIME_READ);
		USART2_StopRead();
		ZL_record(1);
	}
	
	//播报
	SYN_ZL(ZL[0]);
	DJ_camera();
	stop(bobao);
	
	//倒车去岔路
	go_enco(20,-100,-1,0,NULL);
	go_acc_ccd_to_hwt101(50,-100,-350,50,-250,-1,0,0,my_jg1_jg2_gd2,my_gd2_gd8);
	HWT101_enco(8,-200);///////////////////////////////////////////////////////////////
	HWT101_go_rotate(-35,70,NULL);

}

void toZLto4(void)
{
	//去4号平台
	car.Target_dis = 15;
	go_acc(100,60,110,0,1,my_enco);
	car.Target_dis = 20;
	go_acc(80,130,220,0,0,my_enco);
	go_acc(50,200,110,0,0,my_gd1);
	HWT101_Calibrate();//标定有问题，经常车歪，前面需要修改
	
	//上4号平台
	go_enco(30,70,0,0,NULL);
	car.Target_dis = 30;
	HWT101_go(0,75,my_enco);
	
	//在4号平台
	HWT101_go(-6,75,my_qc1);
	DJ_over();
	SYN_FrameInfo(0, (uint8_t *)"[v13][t5]到达四号平台");
	DJ_pt();
	stop(200);
	turn_180();
	DJ_down();
	DJ_camera();
	
	//下4号平台
	HWT101_Calibrate();
	car.Target_dis = 15;
	HWT101_go(1,50,my_enco);
	go_enco(50,50,0,0,NULL);
	go_enco(20,85,0,0,NULL);
	
	//往交通口走
	car.Target_dis = 15;
	go_acc(100,150,200,0,0,my_enco);
	go_acc(100,200,300,0,0,my_huidu1011);
	
	//第二趟记忆
	if(flagroad==3||flagroad==4)
	{
		go_enco(65,300,0,0,NULL);
		go_acc(50,300,350,0,1,my_huidu01);
		go_enco(100,350,0,0,my_huidu1011);
		go_acc(150,350,150,0,0,my_huidu01);
		go_enco(50,110,0,0,my_gd2_gd8);
		go_enco(10,85,0,0,NULL);
	}
	else
	{
		go_enco(65,300,0,0,NULL);
		go_acc(100,300,110,0,0,my_huidu01);
		go_enco(15,85,0,0,my_gd2_gd8);
	}
	//part1结束
	//转弯写在下面一段
}

void JT1(void)
{
	DJ_camera();
	//过交通灯路段
	switch(flagroad)
	{
		case 1:	flagroad_1();	break;	//第二趟跑交通灯，直接跑第一趟记录下的路线
		case 2:	flagroad_2();	break;	//第二趟跑交通灯，直接跑第一趟记录下的路线
		case 3:	flagroad_3();	break;	//第二趟跑交通灯，直接跑第一趟记录下的路线
		case 4:	flagroad_4();	break;	//第二趟跑交通灯，直接跑第一趟记录下的路线
		default:flagroad_0();	break;	//第一趟跑交通灯，并记录走的是记号路线
	}
}

void to5(void)//从交通左上角，面向5号平台开始，先撞5号平台，回到交通左上角，并转弯
{
	if(flagroad==3)	//第三种红绿灯情况，把跑到5号平台的代码放在了JT段，这样中间十字路口就不用先减速后加速避免浪费时间
	{
		stop(20);
	}
	else							//其他红绿灯情况，由于需要转弯，所以不能直接跑到5号平台
	{
		go_acc(100,90,280,0,0,my_huidu1011);
		car.Target_dis = 80;
		go_acc(80,280,110,0,0,my_enco);		//到平台
		go_GD(1,80,0,0,NULL);
		HWT101_Calibrate();
	}
	
	//上5平台
	go_enco(30,70,0,0,NULL);
	car.Target_dis = 50;
	HWT101_go(1,75,my_enco);
	
	//在5平台
	HWT101_go(1,75,my_qc1);
	SYN_FrameInfo(0, (uint8_t *)"[v16][t5]到达五号平台");
	DJ_pt();
	stop(200);
	turn_180();
	DJ_down();
	DJ_camera();
	
	//下5平台
	down_platform();	
	DJ_down();
	DJ_camera();
	
	//到part3
	car.Target_dis = 18;
	go_acc(100,40,90,0,0,my_enco);
	go_acc(50,110,280,0,0,my_huidu01);
	go_enco(60,280,0,0,NULL);
	go_acc(100,280,110,0,0,my_huidu01);		//到交通左上角
	go_enco(15,85,0,0,NULL);//停稳//这个地方容易走多
	
	HWT101_Calibrate();
	HWT101_go_rotate(45,60,NULL);
	stop(20);//建议有一个延时，不然可能会出现只转部分角度
	HWT101_Calibrate();
	HWT101_go_rotate(45,60,my_huidu56);
}

void toZLtoD(void)//已回到交通左上角，面向直立景点，《撞直立景点，到丁字路口》
{
	//直立景点start
	qi_bu();
	go_acc(50,90,110,0,0,my_huidu1011);
	go_enco(13,85,0,0,NULL);//停稳
	
	//开启识别
	if(!ZL[1])
	{
		DJ_ready();
		//DJ_camera_look();
		USART2_StartRead();//快到达直立景点时，开启串口，接收摄像头数据
	}
	HWT101_Calibrate();
	HWT101_go_rotate(-45,65,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(-45,65,my_huidu56);					//第三个直角转弯
	
	go_enco(5,50,0,0,NULL);/////////////////////////////////////////////////////////////////////注意！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	
	//记录
	if(!ZL[1] || ZLflag)
	{
		car.Direct = 0;
		stop_read(TIME_READ);
		USART2_StopRead();
		ZL_record(2);
	}
	go_enco(40,70,0,0,my_qc1);
	
	
	//播报
	SYN_ZL(ZL[1]);
	DJ_camera();
	stop(bobao);
	
	//直立景点end
	HWT101_go(0,-60,my_gd2);
	HWT101_Calibrate();
	HWT101_go_rotate(45,65,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(45,65,my_huidu56);

	//跑到丁字路口，并转弯结束
	car.Target_dis = 40;
	go_acc(100,50,130,0,0,my_enco);
	go_enco(50,130,0,0,my_huidu1011);

	HWT101_Calibrate();
	HWT101_enco(12,90);//陀螺仪停稳
	
	HWT101_Calibrate();
	HWT101_go_rotate(45,70,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(45,70,my_huidu56);
}

void toJTtoZJ(void)//在丁字路口，面向阶梯, 过阶梯，到桥洞之前的直角
{
	qi_bu();
	car.Target_dis = 70;
	go_acc(50,110,250,0,1,my_enco);
	go_acc(80,180,110,0,1,my_gd1);
	
	go_enco(80,90,0,0,NULL);		//过阶梯/////////////////////////////////////////////////
	go_enco(40,70,0,0,NULL);		//过阶梯/////////////////////////////////////////////////
	
	car.Target_dis = 50;
	go_acc(50,150,200,0,0,my_enco);
	go_acc(50,150,110,0,0,my_huidu1011);
	
	HWT101_Calibrate();		
	HWT101_enco(14,85);		//停稳
	
	HWT101_Calibrate();
	HWT101_go_rotate(-45,60,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(-45,60,my_huidu56);		//转弯
}

void toGQtoJTtoD(void)//在中平台桥洞之前的直角, 过桥洞，过阶梯，过两个直角弯
{
	DJ_down();
	//桥洞over
	car.Target_dis = 130;
	go_acc(80,50,150,0,0,my_enco);				//过桥洞
	
	go_acc(50,150,110,0,0,my_huidu1011);		//过桥洞
	HWT101_Calibrate();
	HWT101_enco(14,85);		//停稳
	
	HWT101_Calibrate();
	HWT101_go_rotate(-45,60,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(-45,60,my_huidu56);		//转弯
	
	qi_bu();
	car.Target_dis = 50;
	go_acc(50,110,200,0,1,my_enco);
	go_acc(50,200,110,0,1,my_gd1);		//到阶梯
	
	go_enco(60,90,0,0,NULL);		//过阶梯/////////////////////////////////////////////////
	go_enco(50,70,0,0,NULL);		//过阶梯/////////////////////////////////////////////////
	
	car.Target_dis = 50;
	go_acc(100,50,280,0,1,my_enco);
	go_acc(50,250,110,0,1,my_huidu1011);		//到丁字路口转弯点
	HWT101_Calibrate();
	HWT101_enco(12,85);		//停稳
	
	HWT101_Calibrate();
	HWT101_go_rotate(-45,60,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(-45,60,my_huidu56);		//转弯
	
	qi_bu();
	go((1<<10)|(1<<11),110,0,0,NULL);
//	go_acc(50,50,85,0,0,my_huidu1011);		//平稳起步
	HWT101_Calibrate();
	HWT101_enco(14,85);		//停稳
	
	HWT101_Calibrate();
	HWT101_go_rotate(-45,50,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(-45,50,my_huidu56);
}

void toJSDtomid(void)//在两平台之间的丁字路口，面向中平台《过减速带，上中平台，下平台后，再过减速带，过阶梯》
{
	//往中平台,先减速带
	DJ_down();
	DJ_camera();
	qi_bu();
	go_acc(50,110,130,0,0,my_gd1);	//平稳起步
	go_enco(180,65,0,0,NULL);			//过减速带////////////////////////////////////
	go_GD(1,100,0,0,NULL);					//到达中平台
	
	car.Target_dis=80;
	go_acc(150,100,130,0,0,my_enco);
	HWT101_Calibrate();
	go_acc(150,130,100,0,0,my_no_huidu);
	car.Target_dis=50;
	HWT101_go(0,70,my_enco);
	HWT101_go(0,70,my_qc1);
	DJ_over();
	
	SYN_FrameInfo(0, (uint8_t *)"[v16][t5]到达七号平台");
	DJ_pt();
	stop(200);
	HWT101_Calibrate();
	HWT101_go_rotate(90,45,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(92,45,NULL);
	
	HWT101_Calibrate();
	DJ_down();
	DJ_camera();
	
	car.Target_dis=20;
	HWT101_go(0,50,my_enco);

	go_enco(10,20,0,0,NULL);			//平稳下坡////////////////这里起始最好40，否则车尾会翘//40
	go_enco(20,50,0,0,NULL);
	go_enco(30,80,0,0,NULL);			//加速平稳下坡//80
	go_enco(40,130,0,0,NULL);			//加速下坡//130
	go_enco(20,150,0,0,NULL);			//加速下坡//150
	DJ_down();
	go_GD(1,130,0,0,NULL);				//到减速带
	
	go_enco(190,60,0,0,NULL);			//过减速带////////////////////////////////////
	
	car.Target_dis = 30;
	go_acc(80,100,270,0,1,my_enco);
	go_acc(80,220,110,0,1,my_gd1);//到阶梯
		
	go_enco(70,75,0,0,NULL);			//过阶梯/////////////////////////////////////
	go_enco(55,75,0,0,NULL);
}

void tobig(void)//过完阶梯后《上大平台，下大平台后，岔路口转弯》
{
	DJ_camera();
	//往大平台走
	go_acc(80,75,230,0,0,my_huidu1011);	//冲到岔路口
//	DJ_sp();
	go_acc(50,200,100,0,0,my_gd1);				//减速到大平台

	HWT101_Calibrate();
	car.Target_dis=80;
	go_acc(150,110,140,0,0,my_enco);
	go_acc(150,140,110,0,0,my_no_huidu);		//////////////////////////
	
	car.Target_dis=40;
	HWT101_go(0,70,my_enco);
	HWT101_go(0,70,my_gd1);
	
	car.Target_dis=80;
	go_acc(150,90,130,0,0,my_enco);
	go_acc(150,130,90,0,0,my_no_huidu);
	
	car.Target_dis=25;
	HWT101_go(0,70,my_enco);
	
	HWT101_go(15,70,my_qc1);
	
	DJ_over();
	
	SYN_FrameInfo(0, (uint8_t *)"[v16][t5]到达八号平台");
	DJ_pt();
	stop(300);
	
	HWT101_go_rotate(-90,30,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(-88,30,NULL);			//八号平台的角度--------------																		//顶部平台转弯
	
	DJ_down();
	DJ_camera();
	HWT101_Calibrate();									
	car.Target_dis=15;
	HWT101_go(0,50,my_enco);
	
	go_enco(20,20,0,0,NULL);			//下第一段坡//落稳////////////////还要调
	go_enco(10,50,0,0,NULL);
	go_enco(30,80,0,0,NULL);
	go_enco(30,110,0,0,my_no_huidu);
	HWT101_Calibrate();
	
	car.Target_dis=50;
	HWT101_go(1,90,my_enco);
	
	go_enco(15,20,0,0,NULL);			//下第二段坡//落稳////////////////还要调
	go_enco(15,50,0,0,NULL);
	go_enco(30,100,0,0,NULL);
	go_enco(70,130,0,0,NULL);
	go_enco(70,130,0,0,my_huidu01);
	
	go_enco(28,70,0,0,NULL);			//落稳
	
	HWT101_Calibrate();
	HWT101_go_rotate(-25,60,NULL);
}

void tohui(void)//开始倒车《过五个直角，两个直立景点》
{
	//倒车去桥洞
	DJ_down();
	HWT101_Calibrate();
	car.Target_dis = 20;
	HWT101_back(0,-100,my_enco);
	go_acc_ccd_to_hwt101(50,-100,-400,50,-250,-1,0,0,my_jg1,my_gd2_gd8);
	
	HWT101_Calibrate();
	HWT101_go_rotate(-40,80,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(-40,80,my_huidu56);
	
	qi_bu();
	car.Target_dis = 140;
	go_acc(50,90,120,0,0,my_enco);		//过桥洞
	go((1<<10)|(1<<11),120,0,0,NULL);		
	HWT101_Calibrate();
	HWT101_enco(14,85);		//停稳
	
	HWT101_Calibrate();
	HWT101_go_rotate(-95,50,my_huidu56);
	
	//第一个直角转弯
	qi_bu();
	car.Target_dis = 55;
	go_acc(50,90,270,0,1,my_enco);
	go_acc(50,230,110,0,1,my_huidu1011);
	HWT101_Calibrate(); 
	HWT101_enco(12,85);		//停稳								
	//到第二个直角转弯点
	HWT101_Calibrate();
	HWT101_go_rotate(-95,40,my_huidu56);//第二个直角转弯
	
	qi_bu();
	car.Target_dis = 110;
	go_acc(50,90,100,0,0,my_enco);		//过桥洞
	
	car.Target_dis = 30;
	go_acc(50,120,270,0,0,my_enco);
	go_acc(50,250,110,0,0,my_huidu1011);
	HWT101_Calibrate();
	HWT101_enco(13,85);		//停稳
	
	if(!ZL[2])
	{
		DJ_ready();
//		DJ_camera_look();
		USART2_StartRead();
	}
	//到直立景点转弯点				
	HWT101_Calibrate();
	HWT101_go_rotate(-45,60,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(-45,60,my_huidu56);
	
	go_enco(5,50,0,0,NULL);
	if(!ZL[2] || ZLflag)
	{
		stop_read(TIME_READ);
		car.Direct = 0;
		USART2_StopRead();
		ZL_record(3);
	}
	go_enco(40,70,0,0,my_qc1);
	
	SYN_ZL(ZL[2]);
	DJ_camera();
	stop(bobao);
	
	HWT101_Calibrate();
	HWT101_go(0,-70,my_gd2);		//倒车
		
	HWT101_Calibrate();
	HWT101_go_rotate(45,60,NULL);				//转弯
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(45,60,my_huidu56);	//第三个直角转弯     出直立景点
	
	qi_bu();
	go_acc(50,110,130,0,1,my_huidu1011);
	HWT101_Calibrate();
	HWT101_enco(12,85);		//停稳							
	//到第四个直角转弯点	
	HWT101_Calibrate();
	HWT101_go_rotate(-95,60,my_huidu56);				//第四个直角转弯
	
	
//////////////////////////////////////////跷跷板段////////////////////////////////////////////////////////////////
//	QQB();
	NO_QQB();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////		

	qi_bu();
	go_acc(50,110,130,0,0,my_huidu1011);
	go_enco(14,50,0,0,NULL);							//到直立景点转弯点（第六个直角）
	if(!ZL[3])
	{
		DJ_ready();
//		DJ_camera_look();
		USART2_StartRead();
	}
	HWT101_Calibrate();
	HWT101_go_rotate(-45,65,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(-45,65,my_huidu56);			//直立景点转弯点（第六个直角）转弯	
	
	go_enco(5,50,0,0,NULL);
	HWT101_Calibrate();
	
	if(!ZL[3] || ZLflag)
	{
		stop_read(TIME_READ);
		car.Direct = 0;
		USART2_StopRead();
		ZL_record(4);
	}
	
	go_enco(40,70,0,0,my_qc1);
	
	SYN_ZL(ZL[3]);
	DJ_camera();
	
	stop(bobao);

	HWT101_go(0,-60,my_gd2);								//倒车
		
	HWT101_Calibrate();
	HWT101_go_rotate(-45,60,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(-45,60,my_huidu56);				//转弯
}

void NO_QQB(void)
{
	DJ_camera();
	qi_bu();//起步
	go_acc(100,90,350,0,1,my_huidu01); ////////////////////////////////加减速
	go_enco(120,350,0,1,NULL);
	go_acc(100,320,110,0,1,my_huidu01);

//	go((1<<0)|(1<<1),85,0,1,NULL);//到十字路口//////////////////////////////////////////bug///////////////////////////

	go_enco(17,70,0,0,NULL);								//到第五个直角转弯点（重点调、、、、、、
	stop(20);
		
	HWT101_Calibrate();
	HWT101_go_rotate(-45,60,NULL);				
	HWT101_Calibrate();
	HWT101_go_rotate(-45,60,my_huidu56);					//第五个直角转弯
}

void QQB(void)
{
	qi_bu();//起步
	go_acc(50,90,180,0,0,my_huidu01);
	car.Target_dis = 60;
	go_acc(50,180,100,0,0,my_enco);
	
	HWT101_Calibrate();
	go_GD(7,100,0,-1,NULL);
	HWT101_go_rotate(90,35,NULL);		//上跷跷板，转的差不多
			
	HAL_Delay(300);
	
	car.Target_dis = 45;
	HWT101_GD(90,50,0,0,my_enco);
	
//	car.Target_dis = 30;
//	HWT101_go(105,50,my_enco);
//	
//	go_enco(80,85,0,-1,NULL);
//	go((1<<0)|(1<<1),85,0,0,NULL);
//	go_enco(15,85,0,0,NULL);
//	
//	HWT101_Calibrate();
//	HWT101_go_rotate(45,45,NULL);
//	HWT101_Calibrate();
//	HWT101_go_rotate(45,45,my_huidu56);
//	
//	go_acc(50,40,100,0,0,my_gd1);
//	up_platform();
//	SYN_FrameInfo(0, "[v13][t5]到达六号平台");
//	DJ_pt();
//	stop(200);
//	turn_180();
//	down_platform();
//	
//	go_acc(50,50,85,0,0,my_huidu01);
//	HWT101_Calibrate();
//	HWT101_enco(14,85);
//	
//	HWT101_Calibrate();
//	HWT101_go_rotate(87,35,my_huidu56);		//学姐说90度有点多了，这里是往外扩的
//	
//	go_GD(7,85,0,0,NULL);	//四分之一个半圆的巡线，一直到第二个QQB
//	
//	HWT101_Calibrate();
//	HWT101_go_rotate(14,35,NULL);	//上跷跷板，转的差不多
//	
//	HAL_Delay(300);
//	
//	HWT101_Calibrate();
//	car.Target_dis = 60;
//	HWT101_GD(0,50,0,0,my_enco);
//	
//	car.Target_dis = 15;
//	HWT101_go(15,50,my_enco);
//	
//	go_enco(100,85,0,-1,NULL);
////////////////////////////走完整圈了/////////////////////////////
//	qi_bu();//起步
//	car.Target_dis =90;
//	go_acc(50,90,230,0,1,my_enco); ////////////////////////////////加减速
////	go_enco(30,230,0,1,NULL);
//	go_acc(50,180,85,0,0,my_huidu01);
//	
//	go_enco(14,70,0,0,NULL);								//到第五个直角转弯点（重点调、、、、、、
//	stop(20);	
//		
//	HWT101_Calibrate();
//	HWT101_go_rotate(-45,40,NULL);				
//	HWT101_Calibrate();
//	HWT101_go_rotate(-45,40,my_huidu56);					//第五个直角转弯
	
/////////////////////////////////////////////////////////////////////////////////////////////////
	
//	qi_bu();//起步
//	go_acc(50,90,180,0,0,my_huidu01);
//	car.Target_dis = 60;
//	go_acc(50,180,100,0,0,my_enco);	//到
//	
//	go_GD(7,100,0,-1,NULL);
//	HAL_Delay(1000);
//	HWT101_Calibrate();
//	HWT101_go_rotate(9,35,NULL);		//上跷跷板，转的差不多
//	HWT101_Calibrate();
//	HAL_Delay(300);
//	
//	car.Target_dis = 55;
//	HWT101_GD(0,60,0,0,my_enco);		//过第一个跷跷板
//	HAL_Delay(1000);
//	
//	car.Target_dis = 20;
//	HWT101_GD(35,60,0,0,my_enco);
//				while(1){}
//	go_enco(80,85,0,-1,NULL);
//	go((1<<0)|(1<<1),85,0,0,NULL);
//	go_enco(15,85,0,0,NULL);
//	
//	HWT101_Calibrate();
//	HWT101_go_rotate(45,45,NULL);
//	HWT101_Calibrate();
//	HWT101_go_rotate(45,45,my_huidu56);
//	
//	go_acc(50,40,100,0,0,my_gd1);
//	up_platform();
//	SYN_FrameInfo(0, "[v13][t5]到达六号平台");
//	DJ_pt();
//	stop(200);
//	turn_180();
//	down_platform();
//	
//	go_acc(50,50,85,0,0,my_huidu01);
//	HWT101_Calibrate();
//	HWT101_enco(14,85);
//	
//	HWT101_Calibrate();
//	HWT101_go_rotate(87,35,my_huidu56);
//	
//	go_enco(200,85,0,0,my_no_huidu);
//	HWT101_Calibrate();
//	HWT101_go(0,85,my_gd17);
//	HWT101_go_rotate(25,45,NULL);
//	
//	HAL_Delay(300);
//	
//	HWT101_Calibrate();
//	car.Target_dis = 70;
//	HWT101_GD(0,50,0,0,my_enco);
//	
//	car.Target_dis = 15;
//	HWT101_go(15,50,my_enco);
//	
//	go_enco(100,85,0,-1,NULL);
////////////////////////////走完整圈了/////////////////////////////
//	qi_bu();//起步
//	car.Target_dis =90;
//	go_acc(50,90,230,0,1,my_enco); ////////////////////////////////加减速
////	go_enco(30,230,0,1,NULL);
//	go_acc(50,180,85,0,0,my_huidu01);
//	
//	go_enco(14,70,0,0,NULL);								//到第五个直角转弯点（重点调、、、、、、
//	stop(20);	
//		
//	HWT101_Calibrate();
//	HWT101_go_rotate(-45,40,NULL);				
//	HWT101_Calibrate();
//	HWT101_go_rotate(-45,40,my_huidu56);					//第五个直角转弯
}

void JT2(void)//回家
{
	DJ_camera();
	switch(flagroad)
	{
		case 1: flagroad_back1(); break;
		case 2: flagroad_back2(); break;
		case 3: flagroad_back3(); break;
		case 4: flagroad_back4(); break;
	}
}

void toZLto3(void)
{
	DJ_camera();
	//测300  预留出最开始的40cm和车身20cm，实际跑图长度为240	
	car.Target_dis = 20;
	go_acc(100,100,120,0,0,my_enco);
	car.Target_dis = 80;
	go_acc(100,150,320,0,0,my_enco);		//加速
	go_enco(80,280,0,0,NULL);		//主要调这个距离
	DJ_ready();
	//car.Target_dis = 90;
	go_acc(50,250,150,0,0,my_gd7);		//减速，从280->130，50ms，大概给80cm
	
	go_enco(30,80,0,0,my_qc1);
	go_enco(8,40,0,0,my_qc1);
	stop(50);
	
	go_enco(15,-80,-1,0,NULL);
	if(!ZL[4])
	{
		
		USART2_StartRead();
		
	}
	car.Direct = 0;
	if(!ZL[4] || ZLflag)
	{
		//stop_read(500);
		stop_read(TIME_ZL);
		USART2_StopRead();
		ZL_record(5);
	}
	SYN_ZL(ZL[4]);
	DJ_camera();
	
	
	stop(bobao);
	
	go_enco(20,-100,-1,0,NULL);
	go_acc_ccd_to_hwt101(50,-100,-400,50,-250,-1,0,0,my_jg2,my_gd2_gd8);
	HWT101_enco(8,-200);
	HWT101_Calibrate();
	HWT101_go_rotate(27,40,NULL);
	
	//测240
	qi_bu();
	car.Target_dis = 80;
	go_acc(100,90,320,0,-1,my_enco);
	go_enco(40,320,0,0,NULL);
	//car.Target_dis = 80;
	go_acc(100,280,110,0,0,my_gd1);

	up_platform();										//上平台
	SYN_FrameInfo(0, (uint8_t *)"[v16][t5]到达三号平台");
	DJ_pt();
	stop(200);
	turn_180();																			//平台转弯
	down_platform();																	//下平台
	DJ_camera();
	
	//直线回家
	qi_bu();
	go_acc(100,90,320,0,0,my_huidu1011);
	go_enco(10,320,0,0,NULL);			//原本280速给60cm//现在300速给50cm 
	
	go_acc(80,300,110,0,0,my_huidu01);								//减速准备左巡
	go_enco(50,85,0,-1,NULL);													//左巡进入回家路口的岔路
	//减速带
	
	go_acc(50,85,130,0,-1,my_gd1);
	go_enco(100,70,0,0,NULL);						//过小减速带////////////////////////////////////
	go_acc(50,70,130,0,0,my_huidu01);		//准备右巡
	go_enco(30,85,0,1,NULL);						//右巡
}

void gohome(void)
{
	go_acc(50,50,100,0,1,my_gd1);				//到家门口
	
	go_enco(20,75,0,0,NULL);
	HWT101_Calibrate();	
	go_enco(30,75,0,0,my_no_huidu);		//上坡
	DJ_over();
	car.Target_dis = 43;
	HWT101_go(1,75,my_enco);
	
	if(BackOnceFlag)
	{
		HWT101_go_rotate(90,40,NULL);
		HWT101_Calibrate();
		HWT101_go_rotate(87,40,NULL);
		BackOnceFlag = 0;
	}
}

void flagroad_0(void)
{
	HWT101_Calibrate();
	HWT101_go_rotate(86,50,NULL);
	
	qi_bu();
	car.Target_dis = 20;
	go_acc(100,110,200,0,0,my_enco);
	HWT101_Calibrate();						//这是倒车时侯用的标定
	go_acc(50,150,100,0,0,my_huidu01);
	stop(350);
	
	if(HSL())  								//[1]是绿灯 就让接着去下一个平台
	{
		car.Target_dis = 50;
		go_acc(50,70,230,0,0,my_enco);
		go_acc(70,230,110,0,0,my_huidu01);
		go_enco(15,50,0,0,NULL);			//停稳
		HWT101_Calibrate();
		HWT101_go_rotate(90,55,NULL);
		qi_bu();
		flagroad = 1;						//线路一
		
	}
	else									//[1]不是绿灯
	{
		go_acc_ccd_to_hwt101(50,-100,-200,10,-250,-1,0,0,my_jg1_gd2,my_gd2_gd8);
		
		HWT101_Calibrate();
		HWT101_go_rotate(-50,100,NULL);
		qi_bu();
		car.Target_dis = 20;
		go_acc(80,110,200,0,0,my_enco);
		HWT101_Calibrate();
		go_acc(80,180,110,0,0,my_huidu01);
		
		stop(350);
		
		if(HSL()) 							//[2]是绿灯 就让接着去下一个平台
		{
			car.Target_dis = 30;
			go_acc(100,85,200,0,0,my_enco);
			go_acc(50,200,110,0,0,my_huidu01);
			go_enco(15,85,0,0,NULL);
			stop(20);
			
			HWT101_Calibrate();
			HWT101_go_rotate(90,100,NULL);
			
			qi_bu();
			car.Target_dis = 60;
			go_acc(100,110,250,0,0,my_enco);
			go_acc(50,250,110,0,0,my_huidu01);
			go_enco(50,85,0,-1,NULL);		//左巡
			flagroad = 2;					//线路二
			
		}
		else								//[2]不是绿灯
		{
			go_acc_ccd_to_hwt101(50,-100,-200,10,-250,-1,0,0,my_jg1_gd2,my_gd2_gd8);
			
			HWT101_Calibrate();  
			HWT101_go_rotate(-45,100,NULL);
			
			qi_bu();						//起步
			go_acc(100,110,300,0,1,my_huidu1011);
			go_enco(70,300,0,0,NULL);
			go_acc(80,230,110,0,0,my_huidu01);
			go_enco(27,85,0,0,NULL);
			stop(20);

			HWT101_Calibrate();
			HWT101_go_rotate(90,60,NULL);
			
			qi_bu();
			car.Target_dis = 20;
			go_acc(100,110,220,0,0,my_enco);
			HWT101_Calibrate();				//这是倒车时侯用的标定
			go_acc(50,220,110,0,0,my_huidu01);
			stop(350);
			if(HSL()) 						//[3]是绿灯 就让接着去下一个平台
			{
				car.Target_dis = 60;
				go_acc(100,70,220,0,0,my_enco);
				go_acc(80,220,110,0,0,my_huidu01);
				go_enco(25,50,0,0,my_gd2);	//停稳
				HWT101_Calibrate();
				HWT101_go_rotate(45,60,NULL);
				stop(50);
				HWT101_Calibrate();
				HWT101_go_rotate(45,60,my_huidu56);
				qi_bu();
				go_acc(50,110,160,0,0,my_gd1);
				go_enco(190,65,0,0,NULL);	//过减速带////////////////////////////////////75会卡轮子
				
				go_acc(80,120,300,0,0,my_huidu01);//直接冲向5号平台
				go_enco(30,300,0,0,NULL);
				go((1<<10)|(1<<11),280,0,0,NULL);	//到中线
				car.Target_dis = 100;
				go_acc(80,250,110,0,0,my_enco);	//到平台
				go_GD(1,80,0,0,NULL);
				HWT101_Calibrate();
				flagroad = 3;				//线路三
				
			}
			else							//[3]不是绿灯
			{
				go_acc_ccd_to_hwt101(50,-100,-200,10,-250,-1,0,0,my_jg1_gd2,my_gd2_gd8);
				
				HWT101_Calibrate();
				HWT101_go_rotate(40,100,NULL);																	//右下
				//倒车结束
				qi_bu();
				car.Target_dis = 45;
				go_acc(150,110,300,0,0,my_enco);
				go((1<<0)|(1<<1),300,0,0,NULL);
				go_enco(30,300,0,0,NULL);
				go((1<<0)|(1<<1),300,0,0,NULL);
				go_enco(90,300,0,0,NULL);
				car.time=150;
				go_acc(150,250,110,0,-1,my_time);//可能存在灰度没检测到/速度没减下来，导致没有左巡
				go_enco(50,100,0,-1,my_huidu01);
				go_enco(80,80,0,-1,NULL);
				flagroad = 4;				//线路四
				
			}
		}
	}
}

void flagroad_1(void)
{
	HWT101_Calibrate();
	HWT101_go_rotate(90,50,NULL);
	
	qi_bu();
	go_acc(50,110,280,0,0,my_huidu01);
	go_enco(20,280,0,0,NULL);
	go_acc(80,250,130,0,0,my_huidu01);
	go_enco(15,70,0,0,NULL);
	HWT101_Calibrate();
	HWT101_go_rotate(90,60,NULL);
	qi_bu();
}

void flagroad_2(void)
{
	HWT101_Calibrate();
	HWT101_go_rotate(30,80,NULL);
	
	qi_bu();
	car.Target_dis = 20;
	go_acc(80,110,250,0,0,my_enco);
	HWT101_Calibrate();
	go_acc(80,250,320,0,0,my_huidu01);
	car.Target_dis = 30;
	go_acc(100,280,220,0,0,my_enco);
	go_acc(80,180,110,0,0,my_huidu01);
	go_enco(15,85,0,0,my_gd2_gd8);
	stop(20);
	
	HWT101_Calibrate();  
	HWT101_go_rotate(90,60,NULL);

	qi_bu();
	car.Target_dis = 40;
	go_acc(100,110,320,0,0,my_enco);
	go_acc(80,300,110,0,0,my_huidu01);
	go_enco(50,75,0,-1,NULL);//左巡
}

void flagroad_3(void)
{
	HWT101_Calibrate();
	HWT101_go_rotate(45,60,NULL);
	stop(50);
	HWT101_Calibrate();
	HWT101_go_rotate(45,60,my_huidu56);
	qi_bu();
	go_acc(50,110,300,0,0,my_huidu01);
	go_enco(20,280,0,0,NULL);
	go_acc(80,250,110,0,0,my_huidu01);
	go_enco(25,70,0,0,my_gd2);
	
	HWT101_Calibrate();
	HWT101_go_rotate(45,60,NULL);
	stop(50);
	HWT101_Calibrate();
	HWT101_go_rotate(45,60,NULL);
	
	qi_bu();
	go_acc(50,110,160,0,0,my_gd1);
	go_enco(190,65,0,0,NULL);				//过减速带//75会卡轮子
	go_enco(5,100,0,0,NULL);
	go_acc(80,120,300,0,0,my_huidu01);		//直接冲向5号平台
	go_enco(50,300,0,0,NULL);
	go((1<<10)|(1<<11),280,0,0,NULL);			//到中线
	car.Target_dis = 100;
	go_acc(80,250,110,0,0,my_enco);			//到平台
	go_GD(1,80,0,0,NULL);
	HWT101_Calibrate();
}

void flagroad_4(void)
{
	HWT101_Calibrate();
	HWT101_go_rotate(135,60,NULL);																	//右下
	
	qi_bu();
	car.Target_dis = 45;
	go_acc(150,110,300,0,0,my_enco);
	go((1<<0)|(1<<1),320,0,0,NULL);
	go_enco(30,350,0,0,NULL);
	go((1<<0)|(1<<1),320,0,0,NULL);
	go_enco(90,300,0,0,NULL);
	car.time=150;
	go_acc(150,250,110,0,-1,my_time);		//可能存在灰度没检测到/速度没减下来，导致没有左巡
	go_enco(50,100,0,-1,my_huidu01);
	go_enco(80,85,0,-1,NULL);
}

void flagroad_back1(void)
{
	car.Target_dis = 30;
	go_acc(50,50,150,0,0,my_enco);
	go_acc(50,130,100,0,0,my_huidu01);
	HWT101_Calibrate();
	HWT101_enco(13,85);		//停稳
	
	HWT101_Calibrate();
	HWT101_go_rotate(-95,60,NULL);				//左下路口转弯

	qi_bu();
	go_acc(50,100,130,0,0,my_gd1);
	go_enco(160,60,0,0,NULL);			//过减速带////////////////////////////////////75会卡轮子
	
	car.Target_dis = 20;
	go_acc(50,120,220,0,0,my_enco);
	go_acc(50,180,110,0,0,my_huidu1011);
	HWT101_Calibrate();
	HWT101_enco(16,85);		//停稳

	HWT101_Calibrate();
	HWT101_go_rotate(45,60,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(45,60,my_huidu56);				//转弯					//右下路口转弯
	
	////////////////////////////////////
	car.Target_dis = 20;
	go_acc(80,40,90,0,0,my_enco);
	go_acc(80,120,300,0,0,my_huidu1011);		////越过4号指示牌//*******************************************
	go_enco(30,300,0,0,NULL);
	go_acc(50,280,130,0,0,my_huidu1011);
	HWT101_Calibrate();
	HWT101_enco(13,85);		//停稳					
	
	HWT101_Calibrate();
	HWT101_go_rotate(85,60,my_huidu56);					//右上路口转弯
	
	qi_bu();
	go_enco(20,85,0,1,NULL);
	go_acc(100,100,350,0,0,my_huidu1011);	//这里的右巡1保证巡线正常
	go_enco(70,320,0,0,NULL);
	go_acc(80,280,110,0,1,my_huidu01);
	go_enco(50,85,0,1,NULL);							//盲走一段，左巡进入岔路口，已经拐进岔路了
}

void flagroad_back2(void)
{
	car.Target_dis = 25;
	go_acc(50,50,150,0,0,my_enco);
	go_acc(50,130,90,0,0,my_huidu01);
	HWT101_Calibrate();
	HWT101_enco(13,85);		//停稳

	HWT101_Calibrate();
	HWT101_go_rotate(-43,60,NULL);
	
	qi_bu();
	go_enco(20,90,0,0,NULL);
	go_acc(100,90,320,0,0,my_huidu1011);		//到中点
	go_enco(20,320,0,0,NULL);							//过线
	go((1<<10)|(1<<11),320,0,0,NULL);			//到交通灯
	go_enco(20,280,0,0,NULL);							//过线
	go_acc(100,250,85,0,0,my_huidu01);		//减速
	
	car.Target_dis=25;		//原25感觉巡线平行//35会不会好一点
	HWT101_Calibrate();
	HWT101_go(0,85,my_enco);							//停稳
	
	HWT101_Calibrate();
	HWT101_go_rotate(60,60,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(60,60,my_huidu56);
	
	go_enco(20,85,0,1,NULL);
	go_acc(100,100,350,0,0,my_huidu1011);		//到“回家路口”				
	go_enco(60,320,0,0,NULL);	
	go_acc(80,280,110,0,1,my_huidu01);		//即将到达左上岔路口
	go_enco(55,85,0,1,NULL);							//盲走一段，左巡进入岔路口，已经拐进岔路了
}

void flagroad_back3(void)
{
	qi_bu();
	go_acc(100,110,320,0,0,my_huidu01);	
	go_enco(45,320,0,0,NULL);						
	go((1<<0)|(1<<1),300,0,0,NULL);		//到达红绿灯监测点
	go_enco(40,280,0,0,NULL);			//越过4号指示牌
	go_acc(100,250,110,0,0,my_huidu1011);	//到达右上路口
	HWT101_Calibrate();
	HWT101_enco(24,85);		//停稳
	stop(50);

	HWT101_Calibrate();
	HWT101_go_rotate(50,60,my_huidu56);		//右上路口转弯

	go_enco(25,85,0,1,NULL);							//盲走一段，左巡进入岔路口，已经拐进岔路了
}

void flagroad_back4(void)
{
	car.Target_dis = 30;
	go_acc(50,50,150,0,0,my_enco);
	go_acc(50,130,100,0,0,my_huidu01);
	HWT101_Calibrate();
	HWT101_enco(13,85);		//停稳

	HWT101_Calibrate();
	HWT101_go_rotate(-42,60,NULL);

	car.Target_dis = 30;
	go_acc(100,90,120,0,0,my_enco);	//起步
	car.Target_dis=80;
	go_acc(100,150,300,0,0,my_enco);
	go_acc(50,280,130,0,0,my_huidu1011);		//到中点
	go_enco(12,86,0,0,NULL);
	
	HWT101_Calibrate();
	HWT101_go_rotate(60,60,NULL);
	stop(20);
	HWT101_Calibrate();
	HWT101_go_rotate(60,60,my_huidu56);
	
	qi_bu();
	car.Target_dis=90;
	go_acc(100,110,300,0,0,my_enco);
	go_acc(50,280,130,0,0,my_huidu01);

	go_enco(45,85,0,0,NULL);
}

void part1_pro(void)
{	
	LBridge();
	LBto2();
	to2toZL();
	toZLto4();
}

void part2_pro(void)
{
	JT1();
	to5();
}

void part3_pro(void)
{
	toZLtoD();
	toJTtoZJ();
	toGQtoJTtoD();
	toJSDtomid();
	tobig();
	tohui();
}

void part4_pro(void)
{
	JT2();
	toZLto3();
	gohome();
}
