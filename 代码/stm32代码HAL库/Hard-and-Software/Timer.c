#include "sys.h"

//htim6,htim7,htim10任意一个定时器溢出，就进入回调函数，不同定时器进中断*htim不同，6->控制，7->采集，10->计时
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	static int flag1=0,flag2=0,flag3=0;
	if(htim == &htim6)	/////控制用定时器
	{
		if(car.state!=0)		//控制运动状态标志位////////////////////////
		{
			if(car.callBack)//实际：如果有回调函数//如果car.callBack得到callBaack的地址，则判定为真
			{
				if(car.callBack())//如果返回1
				{
					car.state=0;//退出中断
				}
			}
			if(car.state==1)//go
			{
					for(int i=0;i<=11;i++)
					{															//car.ADnum（二进制格式）：检测到几号灰度就结束go函数;		1号灰度：car.ADnum = 0000 0000 0001;  2号灰度：car.ADnum = 0000 0000 0010
						if((car.ADnum & (1U<<i)))		//匹配i：就是让1左移i位，再与car.ADnum进行匹配，匹配对了，i就知道是第几个灰度了		//1U:1unsigned(无符号整数1)		//<<i:左移 i 位		//&:11得1
						{
							if(AD_data[i]>ADW[i])			//判断i：第i个灰度值>检测阈值3200
							{
								car.state=0;						//go函数结束
								break;
							}
						}
					}
			}
			if(car.state==2)//HWT101_go
			{
				if(car.HWT101_getOnce)																//获取更新标志位
				{
					ReadHWT101();																				//读取一次陀螺仪数据	car.yaw -> car.Angle
					car.HWT101_getOnce=0;																//清除标志位
					PID_calc_A(&pid_A,car.Target_Angle,car.Angle);			//PID计算_角度（计算出output）
				}
			}
			if(car.state==3)//HWT101_go_roate
			{
				if(car.HWT101_getOnce)
				{
					ReadHWT101();//读取
					car.HWT101_getOnce=0;
					PID_calc_A(&pid_R,car.Target_Angle,car.Angle);
					printf("%f,%f,%f\n",pid_R.output,pid_1.output,car.Target_Angle-car.Angle);
					pid_R.output*=car.Target;
					LIMIT_MIN_MAX((pid_R.output),-100,100);
					if(pid_R.output>0&&pid_R.output<50)
					{
						pid_R.output=50;
					}
					if(pid_R.output<0&&pid_R.output>-50)
					{
						pid_R.output=-50;
					}

				}
				if((car.Target_Angle-car.Angle)<2.0f&&(car.Target_Angle-car.Angle)>-2.0f)//如果与预设值差值小停车
					{
						car.state=0;
					}
			}
			if(car.state==5)//go_enco
			{
				if(car.Target_dis-car.enco_counter<=1)
				{
					car.state=0;
				}
			}
			if(car.state==6)//go_time
			{
				if(car.time<=0)
				{
					HAL_TIM_Base_Stop_IT(&htim10);
					car.state=0;
				}
			}
			if(car.state==7)//go_gd
			{
				if(GD(car.gd_id)==0)
				{
					car.state=0;
				}
			}
			if(car.state==9)//HWT101_gd
			{
				if(car.HWT101_getOnce)
				{
					ReadHWT101();//读取
					car.HWT101_getOnce=0;
					if (!GD6 && !GD5) car.Target_Angle=car.Target_Angle;
					else if (!GD5)  car.Target_Angle-=0.5f;
					else if (!GD6)  car.Target_Angle+=0.5f;
					LIMIT_MIN_MAX(car.Target_Angle,car.Target_Angle_1-7,car.Target_Angle_1+7);
					PID_calc_A(&pid_A,car.Target_Angle,car.Angle);
				}
			}
			if(car.state==10)//HWT101_gd
			{
				if(car.HWT101_getOnce)
				{
					ReadHWT101();//读取
					car.HWT101_getOnce=0;
					if (!GD6 && !GD5) car.Target_Angle=car.Target_Angle;
					else if (!GD5)  car.Target_Angle+=1.2f;
					else if (!GD6)  car.Target_Angle-=1.2f;
					LIMIT_MIN_MAX(car.Target_Angle,car.Target_Angle_1-10,car.Target_Angle_1+10);
					PID_calc_A(&pid_A,car.Target_Angle,car.Angle);
				}
			}
		}

		if (car.state!=0)
		{
			if (car.adjust==1)		//控制进入时间(20ms)标志位
			{
				car.adjust=0;
				if (car.line)				//巡线开关标志位
				{
					if(car.Direct==0)
					{
						static int ccdcnt=0;
						if(ccdcnt>=4)//4ms矫正一次
						{
							ccdcnt=0;
							huidu_ccd_pid();
							car.pid_able=0;
						}
						ccdcnt++;
					}
					else
					{
						static int ccdcnt1=0;
						if(car.delay_time>=7)//曝光时间正常，直接循线
						{
							ccdcnt1=0;
							if(car.pid_able)//满足Pid条件
							{
								huidu_ccd_pid();
								car.pid_able=0;
							}
						}
						else//时间过短
						{
							if(ccdcnt1>=7)
							{
								ccdcnt1=0;
								if(car.pid_able)//满足Pid条件
								{
									huidu_ccd_pid();
									car.pid_able=0;
									}
							}
							ccdcnt1++;
						}
					}
				}
				if(car.state!=3)	//非转弯
				{
						float line_out = car.line ? car.OUT : 0.0f;
						PID_calc_V(&pid_1,(car.Target-pid_A.output-pid_R.output+pid_F.output)*(1.0f-line_out),(float)velocity[1]);	//给轮子一套调速PID//1号轮的PID结构体，目标速度，实际速度
						PID_calc_V(&pid_2,(car.Target+pid_A.output+pid_R.output+pid_F.output)*(1.0f+line_out),(float)velocity[2]);
						PID_calc_V(&pid_3,(car.Target-pid_A.output-pid_R.output+pid_F.output)*(1.0f-line_out),(float)velocity[3]);
						PID_calc_V(&pid_4,(car.Target+pid_A.output+pid_R.output+pid_F.output)*(1.0f+line_out),(float)velocity[4]);
				}
				else
				{
					PID_calc_V(&pid_1,-pid_R.output,(float)velocity[1]);	//陀螺仪转弯，用同样的PID，只不过目标速度的计算只涉及转角度PID
					PID_calc_V(&pid_2,+pid_R.output,(float)velocity[2]);
					PID_calc_V(&pid_3,-pid_R.output,(float)velocity[3]);
					PID_calc_V(&pid_4,+pid_R.output,(float)velocity[4]);
				}
				Serial.Vofa("%d,%d,%d,%d",velocity[1],velocity[2],velocity[3],velocity[4]);
				move_control((int)pid_1.output,(int)pid_2.output,(int)pid_3.output,(int)pid_4.output);	//把前面算出来的output扔给电机跑
			}
		}
		else
		{
			move_control(0,0,0,0);
		}
	}
	if(htim == &htim7)
	{
		flag1++;flag2++;flag3++;
		if(flag1>=2)//陀螺仪5ms
		{
			// 按历史顺序依次“后移”
			car.yaw_20 = car.yaw_19;
			car.yaw_19 = car.yaw_18;
			car.yaw_18 = car.yaw_17;
			car.yaw_17 = car.yaw_16;
			car.yaw_16 = car.yaw_15;
			car.yaw_15 = car.yaw_14;
			car.yaw_14 = car.yaw_13;
			car.yaw_13 = car.yaw_12;
			car.yaw_12 = car.yaw_11;
			car.yaw_11 = car.yaw_10;
			car.yaw_10 = car.yaw_9;
			car.yaw_9 = car.yaw_8;
			car.yaw_8 = car.yaw_7;
			car.yaw_7 = car.yaw_6;
			car.yaw_6 = car.yaw_5;
			car.yaw_5 = car.yaw_4;
			car.yaw_4 = car.yaw_3;
			car.yaw_3 = car.yaw_2;
			car.yaw_2 = car.yaw_1;

			ReadHWT101(); // 读取 yaw 角（当前最新值）
			car.yaw_1 = car.yaw;

			flag1=0;
		}
		if(car.Direct==0)//灰度2ms
		{
			static int cntq=0;
			car.ccd_use =0;//ccd复位
			if(cntq>=2)//2ms采集一次
			{
				cntq=0;
				AD_data[0]=AD(0);
				AD_data[1]=AD(1);
				AD_data[2]=AD(2);
				AD_data[3]=AD(3);
				AD_data[4]=AD(4);
				AD_data[5]=AD(5);
				AD_data[6]=AD(6);
				AD_data[7]=AD(7);
				AD_data[8]=AD(8);
				AD_data[9]=AD(9);
				AD_data[10]=AD(10);
				AD_data[11]=AD(11);
			}
			cntq++;
		}
		else if(car.Direct==1)//ccd
		{
			if(flag3>=car.delay_time)//大概要占5ms的时间,一般扫描周期20ms
			{
				if(car.ccd_use<=2)//至少要取值两次，才能用其数值
				{
					Read_128(ADC_CHANNEL_5);//前面的CCD///////////////////////////////////////////////////暂时换到了后面////////////////////////////////////////////////////////////////////////////
					car.pid_able=0;
					car.ccd_use++;
				}
				else
				{
					Read_128(ADC_CHANNEL_5);//前面的CCD///////////////////////////////////////////////////暂时换到了后面////////////////////////////////////////////////////////////////////////////
					Gap_Avr(Pixel, 4);//转换成128/4=32个值
					Aver128(Pixel,0,31);//同时计算了pixel_max
					car.find = CcdFindLine2(Pixel,32,25,car.Line_Search);
					car.pid_able=1;
				}
				//自动曝光
				auto_Exposure();
				flag6times++;//主函调试窗口用到这个
				flag3=0;
			}flag3++;
		}
		else
		{
			if(flag3>=car.delay_time)//大概要占5ms的时间,一般扫描周期20ms
			{
				if(car.ccd_use<=2)//至少要取值两次，才能用其数值
				{
					Read_128(ADC_CHANNEL_4);//后面的CCD///////////////////////////////////////////////////暂时换到了前面////////////////////////////////////////////////////////////////////////////
					car.pid_able=0;
					car.ccd_use++;
				}
				else
				{
					Read_128(ADC_CHANNEL_4);//////////////////////////////////////////////////////////////暂时换到了前面////////////////////////////////////////////////////////////////////////////
					Gap_Avr(Pixel, 4);//转换成128/4=32个值
					Aver128(Pixel,0,31);//同时计算了pixel_max
					car.find = CcdFindLine2(Pixel,32,25,car.Line_Search);
					car.pid_able=1;
				}
				//自动曝光
				auto_Exposure();
				flag6times++;//主函调试窗口用到这个
				flag3=0;
			}flag3++;
		}
		if(flag2>=20)//满速度500//编码器//20ms读一次编码器值
		{
			flag2=0;
			if(car.enco)
			{
				move_encoder();
				if (car.Direct==0)
				{
					car.enco_counter=(encoder[1]+encoder[2]+encoder[3]+encoder[4])/4;	//这个car.enco_counter主要参与private.c中my_enco()函数的计算
				}
				if (car.Direct==1)
				{
					car.enco_counter=(encoder[1]-encoder[2]-encoder[3]+encoder[4])/4;	//这个是前进的模式吧，要减的吗？
				}
				if (car.Direct==-1)
				{
					car.enco_counter=(-encoder[1]-encoder[2]-encoder[3]-encoder[4])/4;//原来是(-encoder[1]+encoder[2]+encoder[3]-encoder[4])/4，原来这个好像不对
				}
			}
			move_velocity();
			car.HWT101_getOnce=1;//标志位置一
			car.adjust=1;
		}
	}
	if(htim == &htim10)
	{
		car.time--;
		if(car.time<=0)
		{
			HAL_TIM_Base_Stop_IT(&htim10);
			car.state=0;
		}
	}



}

