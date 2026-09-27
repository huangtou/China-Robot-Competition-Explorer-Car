#include "sys.h"

uint8_t Pixel[128];
uint8_t pixel_max;

void delayus(u32 m)
{
	m*=100;
	while(m--);
}

uint32_t ME_POW(uint32_t j,uint32_t n)//n次方函数//j的n次方
{
	int i;
	uint32_t ans=1;
	
	for(i=0;i<n;i++)
	{
		ans*=j;
	}
	return ans;
}

int aboutEqual(float a,float b,float c)//约等于
{
	if(fabs(a-b)<=c)
		return 1;
	else
		return 0;
}

uint8_t Read_ADC_data(ADC_HandleTypeDef* hadc, uint32_t Channel) 
{
    uint32_t adcValue;

    // 配置ADC通道
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = Channel; 
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES; // 采样时间
    HAL_ADC_ConfigChannel(&hadc2, &sConfig);
    // 启动ADC转换
    HAL_ADC_Start(&hadc2);
    // 等待转换完成
    HAL_ADC_PollForConversion(&hadc2,10);
    // 获取转换值
    adcValue = HAL_ADC_GetValue(&hadc2);
    // 停止ADC
    HAL_ADC_Stop(&hadc2);
	
	uint8_t scaled = ((adcValue * 225) / 4095);
    return scaled; 
	
}

//读取128个数据到Pixel数组
void Read_128(uint32_t Channel)//核心
{
	if (Channel==ADC_CHANNEL_4)
	{
		CLK_out=0;           /* CLK = 0 */
		SI_out=0;            /* SI  = 0 */
		uint8_t i;
		static uint8_t ad0,ad1,ad2;
		uint8_t *Pixel_p = Pixel;
		SI_out=1;            /* SI  = 1 */
		delayus(Time_1);
		CLK_out=1;           /* CLK = 1 */
		delayus(Time_1);
		SI_out=0;            /* SI  = 0 */
		delayus(Time_1);

	//Delay 10us for sample the first pixel
		for(i = 0; i < Time_2; i++)
		{                    //更改250，让CCD的图像看上去比较平滑，
		  delayus(1);  		//200ns                  //把该值改大或者改小达到自己满意的结果。
		}

	//Sampling Pixel 1

		*Pixel_p =  Read_ADC_data(&hadc2,CCD2_CHANNEL);//ccd2
		Pixel_p ++ ;
		CLK_out=0;           /* CLK = 0 */
		for(i=0; i<127; i++)
		{
			delayus(Time_1);
			CLK_out=1;       /* CLK = 1 */
			delayus(Time_1);
			//Sampling Pixel 2~128
					ad2=ad1;
					ad1=ad0;
					ad0 = Read_ADC_data(&hadc2,CCD2_CHANNEL);
		   *Pixel_p = ad2*0.3+ad1*0.3+ad0*0.4;//一阶互补滤波
	//		*Pixel_p =ad0;//一阶互补滤波
	//-----------------------------------
			Pixel_p ++ ;
			CLK_out=0;       /* CLK = 0 */
		}
		delayus(Time_1);
		CLK_out=1;           /* CLK = 1 */
		delayus(Time_1);
		CLK_out=0;           /* CLK = 0 */	
	//		Pixel[0]=Pixel[1]=Pixel[2]=Pixel[3]=Pixel[4]=Pixel[5]=0;//前四个点例外//实际中发现的
	}
	else if (Channel==ADC_CHANNEL_5)
	{
		CLK_out_2=0;           /* CLK = 0 */
		SI_out_2=0;            /* SI  = 0 */
		uint8_t i;
		static uint32_t ad0,ad1,ad2;
		uint8_t *Pixel_p = Pixel;
		SI_out_2=1;            /* SI  = 1 */
		delayus(Time_1);
		CLK_out_2=1;           /* CLK = 1 */
		delayus(Time_1);
		SI_out_2=0;            /* SI  = 0 */
		delayus(Time_1);

	//Delay 10us for sample the first pixel
		for(i = 0; i < Time_2; i++)
		{                    //更改250，让CCD的图像看上去比较平滑，
		  delayus(1);  		//200ns                  //把该值改大或者改小达到自己满意的结果。
		}

	//Sampling Pixel 1

		*Pixel_p =  Read_ADC_data(&hadc2,CCD1_CHANNEL);//ccd1
		Pixel_p ++ ;
		CLK_out_2=0;           /* CLK = 0 */
		for(i=0; i<127; i++)
		{
			delayus(Time_1);
			CLK_out_2=1;       /* CLK = 1 */
			delayus(Time_1);
			//Sampling Pixel 2~128
					ad2=ad1;
					ad1=ad0;
					ad0 = Read_ADC_data(&hadc2,CCD1_CHANNEL);
		   *Pixel_p = ad2*0.3+ad1*0.3+ad0*0.4;//一阶互补滤波
	//		*Pixel_p =ad0;//一阶互补滤波
	//-----------------------------------
			Pixel_p ++ ;
			CLK_out_2=0;       /* CLK = 0 */
		}
		delayus(Time_1);
		CLK_out_2=1;           /* CLK = 1 */
		delayus(Time_1);
		CLK_out_2=0;           /* CLK = 0 */	
	//		Pixel[0]=Pixel[1]=Pixel[2]=Pixel[3]=Pixel[4]=Pixel[5]=0;//前四个点例外//实际中发现的
	}
}

uint32_t AvEasy(uint8_t *pixel,int begin,int end)//简单求取平均值
{
	int sum=0;
	int len=end-begin+1;
	
	for(int i=begin;i<=end;i++)
	{
		sum+=pixel[i];
	}
	return sum/len;
}

//保存到Pixel从128变成32中//核心//非常重要！！！！！
uint32_t Gap_Avr(uint8_t* pixel,int gap)
{
	uint32_t i;
	uint32_t num=128/gap;//计算数组元素/未删减的元素
	
	for(i=0;i<num;i++)
	{
//		pixel[i]=Aver128(pixel,i*gap,(i+1)*gap-1);
		pixel[i]=AvEasy(pixel,i*gap,(i+1)*gap-1);
	}
	return num;
}

//128个平均数+附加功能
//额外得出最大值，平均值，左平均值，右平均值
uint16_t Aver128(uint8_t* pixel, uint8_t begin, uint8_t end)
{
    static uint8_t getmin = 0;   // min存入minpixel的次数
    static uint8_t minerror = 0; // min不对劲的次数

    uint32_t i;
    uint32_t mid = (begin + end) / 2;
    uint32_t sum = 0, sum_left = 0, sum_right = 0;
    uint8_t max = 0, min = 255;
    uint8_t left_count = 0, right_count = 0;

    // 取样本总和、最大、最小，同时统计左右平均值
    for(i = begin; i <= end; i++)
    {
        uint8_t val = pixel[i];
        sum += val;

        // 最大最小值只排除两端4点
        if(i > begin + 1 && i < end - 1)
        {
            if(val > max) max = val;
            if(val < min) min = val;
        }
        if(i <= mid)
        {
            sum_left += val;
            left_count++;
        }
        else
        {
            sum_right += val;
            right_count++;
        }
    }

    // 计算平均值
    uint16_t all_av   = sum / (end - begin + 1);
    uint16_t left_av  =sum_left  / left_count;
    uint16_t right_av =sum_right / right_count;

    car.all_av = all_av;
    car.zuo_av = left_av;
    car.you_av = right_av;
    car.max    = max;
    car.min    = min;

    //最小值滤波与bound计算
	uint8_t* min_arr;
	float* min_av_ptr;
//	if(car.Direct == 1)
//	{
//		min_arr = car.minpixel;
//		min_av_ptr = &car.minav;
//	}
//	else
//	{
		min_arr = car.minpixel2;
		min_av_ptr = &car.minav2;
//	}
	if(min <= 240)
	{
		minerror = 0;
		min_arr[getmin] = min;
		getmin = (getmin + 1) % 10;
	}
	else
	{
		minerror++;
	}
	*min_av_ptr = AvEasy(min_arr, 0, 9);

	// bound计算
	float minav = *min_av_ptr;
	if(car.bili < 0.5f)
	{
		if(max - minav >= minav * 3 / 5)
			car.bound = car.bili * max + (1.0f - car.bili) * minav;
		else
			car.bound = (max + 30 <= 255) ? max + 30 : 255;
	}
	else
	{
		car.bound = car.bili * max + (1.0f - car.bili) * minav;
	}
    return all_av;
}

float CcdFindLine2(uint8_t *pixel,const int num,int fly,int8_t way)//数组元素个数，输入Pixel_num，考量前18个元素//way:巡线方式0,1,2,3
{
	int bound = car.bound;
	u32 pill=0;//峰值点的值的总和//重要！！

	static int go_noneflag=0;
	static int go_haveflag=0;
	
	int PID_lost=0;
	float zuo=-1,you=-1;//左边界右边界
	float find = -1;    //两个峰时记录左峰，一个峰时只用这个
	float find2= num;   //两个峰时记录右峰位置

//自适应曝光	
	
/////////////////////////中间巡线///////////////////////////	
	if(way==0)
	{
		const int lef = car.AV2;//=15从中间开始 一共32个
		int i=lef; 
		
		if(pixel[lef]>=bound)
		{
			//向左检测
			while(i>=0)
			{
				if(i>=3)//找下降点
				{
					if(pixel[i]<bound)
					{
						if(pixel[i-1]<=pixel[i])//并且导数大于0//可多添加一项
						{
							zuo=(float)i;//左边界
							break;
						}
					}
					else//如果pixel[i]大于bound
					{
						pill+=pixel[i];//加到峰值里----1
					}
					i--;
				}
				else//边界上2个点另当别论
				{
					if(pixel[0]>=bound&&pixel[1]>=bound)
					{
						pill+=pixel[i];//加到峰值里----2
						zuo=(float)0;
						i=0;
						break;
					}
					else
						i--;
				}
			}
			//向右检测
			while(i<num&&PID_lost==0)
			{
				if(i<num-2)//找下降点
				{
					if(pixel[i]<=bound)
					{
						if(pixel[i+1]<=pixel[i])//并且导数小于于0//可多添加一项
						{
							you=(float)i;//右边界
							find=(zuo+you)/2;
							break;
						}
					}
					else
					{
						pill+=pixel[i];//加到峰值里----1
					}
					i++;
				}
				else//边界上3个点另当别论
				{
					if(pixel[30]>=bound&&pixel[31]>=bound)
					{
						pill+=pixel[i];//加到峰值里----2
						you=(float)(num-1.5);
						i=num-1;
						find=(zuo+you)/2;
						break;
					}
					else
						i++;
				}
			}
		}
		else//第二种,中间不大于界限bound
		{
			while(i>=0)//向左检测
			{
				if(i>=3)//找上升点
				{
					if(pixel[i]>=bound)
					{
						if(pixel[i+1]<=pixel[i])//并且导数小于0//可多添加一项
						{
							pill+=pixel[i];//加到峰值里----1
							you=(float)i;//右边界
							while(i>=0)
							{
								if(pixel[i]<bound)
								{
									zuo=(float)i;//左边界
									find=(zuo+you)/2.0f;
									break;
								}
								else
								{
									pill+=pixel[i];//加到峰值里----2
									i--;
								}
							}
						}
					}
					i--;
				}
				else//边界上2个点另当别论
				{
					if(you!=-1&&(pixel[0]>=bound||pixel[1]>=bound))//如果找到了右边界却没有左边界,继续扫描
					{
						pill+=pixel[0]+pixel[1];//加到峰值里----1
						zuo=(float)0.5;
						i=0;
						find=(zuo+you)/2;
						break;
					}
					else
						i--;
				}
			}
			//向右检测
			i=lef;//赋值
			while(i<num)
			{
				if(i<num-2)//找上升点
				{
					if(pixel[i]>bound)
					{
						if(pixel[i+1]>=pixel[i])//并且导数大于0 //可多添加一项
						{
							pill+=pixel[i];//加到峰值里----1
							zuo=(float)i;//左边界                                                                                                                                                                                                              原神启动
							while(i>=0)
							{
								if(pixel[i]<bound)
								{
									you=(float)i;//左边界
									find2=(zuo+you)/2;
									break;
								}	
								else
									i++;
							}
						}
					}
					i++;
				}
				else//边界上3个点另当别论
				{
					if(zuo!=-1&&(pixel[num-1]>=bound||pixel[num-2]>=bound))
					{
						pill+=pixel[num-1]+pixel[num-2];
						you=(float)(num-1.5);
						i=num-1;//确保退出
						find2=(zuo+you)/2;
						break;
					}
					else
						i++;
				}
			}
			//挑一个靠中心小的作为find
			if((float)lef-find-0.5f<=find2-(float)lef+0.5f)
			{
				find=find;
			}
			else
			{
				find=find2;
			}
			
		}
	
	
	}
	else if(way==-1)//左巡线
	{
		const int lef=car.AV2;//15
		int i=lef;

		i=1;//赋值
		while(i<lef)//向右检测
		{
			if(pixel[i]>=bound)
			{
				if(pixel[i]>=pixel[i-1])//并且导数大于0//可多添加一项
				{
					zuo=(float)i;//左边界
					while(i<lef*3/2)
					{
						if(pixel[i]<bound)
						{
							you=(float)i;//右边界
							find=(zuo+you)/2.0f;
							break;
						}
						else i++;
					}
				}
				if(find!=-1) break;
			}
			i++;
		}
		//如果以上的循环循环到底---没找到
		if(find==-1 && pixel[0]>=bound)
		{
			if(pixel[1]<=pixel[0])
			{
				zuo=0;
				you=2;
				find=(zuo+you)/2.0f;
			}
		}
		if(find==-1 && find2==num && pixel[lef]>=bound)//依旧没找到
		{
			i=lef;
			while(i>=0)//向左检测
			{
				if(i>=3)//找下降点
				{
					if(pixel[i]<bound)
					{
						if(pixel[i-1]<=pixel[i])//并且导数大于0//可多添加一项
						{
							zuo=(float)i;//左边界
							break;
						}
					}
					i--;
				}
				else//边界上3个点另当别论
				{
					if(pixel[0]>=bound&&pixel[1]>=bound)
					{
						zuo=(float)0;
						i=0;
						break;
					}
					else
						i--;
				}
			}
			//向右检测
			i=lef;
			while(i<num&&PID_lost==0)
			{
				if(i<num-2)//找下降点
				{
					if(pixel[i]<=bound)
					{
						if(pixel[i+1]<=pixel[i])//并且导数小于于0//可多添加一项
						{
							you=(float)i;//右边界
							find=(zuo+you)/2;
							break;
						}
					}
					i++;
				}
				else//边界上3个点另当别论
				{
					if(pixel[30]>=bound&&pixel[31]>=bound)
					{
						you=(float)(num-1.5);
						i=num-1;
						find=(zuo+you)/2;
						break;
					}
					else
						i++;
				}
			}
		}
		if(find==-1 && find2==num)//如果没有找到线//向右检测
		{
			//向右检测
			i=lef;//赋值
			while(i<num)
			{
				if(i<num-2)//找上升点
				{
					if(pixel[i]>bound)
					{
						if(pixel[i+1]>=pixel[i])//并且导数大于0//可多添加一项
						{
							zuo=(float)i;//左边界
							while(i>=0)
							{
								if(pixel[i]<bound)
								{
									you=(float)i;//左边界
									find2=(zuo+you)/2;
									break;
								}	
								else
									i++;
							}
						}
					}
					i++;
				}
				else//边界上3个点另当别论
				{
					if(zuo!=-1&&(pixel[num-1]>=bound||pixel[num-2]>=bound))
					{
						you=(float)(num-1.5);
						i=num-1;
						find2=(zuo+you)/2;
						break;
					}
					else
						i++;
				}
			}
		}
	}
	else if(way == 2)//右巡线//从中间
	{
		const int lef=car.AV2;
		int i=lef;			

		i=num-1;//赋值
		while(i>lef)//从最右端向中间检测
		{
			if(pixel[i]>=bound)
			{
				if(pixel[i-1]>=pixel[i])//并且导数小于0//可多添加一项
				{
					you=(float)i;//右边界
					while(i>lef/2)
					{
						if(pixel[i]<bound)
						{
							zuo=(float)i;//右边界
							find=(zuo+you)/2.0f;
							break;
							//return find
						}
						else
							i--;
					}
				}
				if(find!=-1)
					break;
			}
			i--;
		}
		
		//如果以上的循环循环到底---没找到
		if(find==-1 && pixel[num-1]>=bound)
		{
			if(pixel[num-2]<=pixel[num-1]||pixel[num-3]<=pixel[num-2])//导数小于0
			{
				zuo=num-1;
				you=num-3;
				find=(zuo+you)/2.0f;
			}
		}
		if(find==-1 && find2==num && pixel[lef]>=bound)//依旧没找到//从中间往两边找
		{
			i=lef;
			while(i>=0)//向左检测
			{
				if(i>=3)//找下降点
				{
					if(pixel[i]<bound)
					{
						if(pixel[i-1]<=pixel[i])//并且导数大于0//可多添加一项
						{
							zuo=(float)i;//左边界
							break;
						}
					}
					i--;
				}
				else//边界上3个点另当别论
				{
					if(pixel[0]>=bound&&pixel[1]>=bound)
					{
						zuo=(float)0;
						i=0;
						break;
					}
					else
						i--;
				}
			}
			//向右检测
			i=lef;
			while(i<num&&PID_lost==0)
			{
				if(i<num-2)//找下降点
				{
					if(pixel[i]<=bound)
					{
						if(pixel[i+1]<=pixel[i])//并且导数小于于0//可多添加一项
						{
							you=(float)i;//右边界
							find=(zuo+you)/2;
							break;
						}
					}
					i++;
				}
				else//边界上3个点另当别论
				{
					if(pixel[0]>=bound&&pixel[1]>=bound)
					{
						you=(float)(num-1.5);
						i=num-1;
						find=(zuo+you)/2;
						break;
					}
					else
						i++;
				}
			}
		}
		if(find==-1 && find2==num)//如果没有找到线//向左检测
		{
			//从中间向左检测
			i=lef;//赋值
			while(i>=1)
			{
				if(pixel[i]>=bound)
				{
					if(pixel[i-1]>=pixel[i])//并且导数小于于0//可多添加一项
					{
						you=(float)i;//左边界
						while(i>=1)
						{
							if(pixel[i]<bound)
							{
								zuo=(float)i;//左边界
								find=(zuo+you)/2.0f;
								break;
								//return find
							}	
							else
								i--;
						}
					}
				}
				i--;
			}	
		}
	
	}
	else if(way==4)//循线方式4//最耗时间
	{
		float find_left=0.0f,find_mid=15.5f,find_right=31.0f;
		int equal=0;
		int flagzuo=1,flagyou=1;
		
		find_left=CcdFindLine2(pixel,32,20,-1);
		find_mid=CcdFindLine2(pixel,32,20,0);		
		find_right=CcdFindLine2(pixel,32,20,1);

		car.find_left=find_left;
		car.find_mid =find_mid;
		car.find_right=find_right;
		
		if(find_left ==-1.0f)
		{
			find_left =0.0f;
			flagzuo=0;
			equal++;
		}
		if(find_mid  ==-1.0f)
		{
			find_mid  =15.5f;
			equal++;
		}
		if(find_right==-1.0f)
		{
			find_right=31.0f;
			flagyou=0;
			equal++;			
		}

		//mid总能找到线，找到临近的就让它回到15.5f
		if(aboutEqual(find_left,find_mid,2.5f))
		{
			find_mid=15.5f;
		}
		else if(aboutEqual(find_left,find_mid,2.5f))
		{
			find_mid=15.5f;
		}
		if(flagzuo && flagyou)//找到了
		{
			find_mid=15.5f;
			car.way4 = 1;
			find=(find_left + find_mid + find_right)/3.0f;
		}
		else if(flagzuo == 0 || flagyou == 0)
		{
			car.way4 = 0;
			find=find_mid;
		}
		zuo=15.5f;you=15.5f;
		if(equal==3)
		{
			find=-1.0f;
			zuo=-1.0f;
			you=-1.0f;
		}
		car.go_find=1;//标记一次
	}
//总结
	if(you-zuo>4)
	{
		if(zuo<=1.0f||you>=num-2.0)
		{
			find=-1;
		}
	}
	if(zuo==-1||you==-1||find==-1)//临终总结
	{
		PID_lost=1;//未检测到一次
		
		if(car.state==6)
		{
			go_noneflag++;//加一次未找到线
			
			if(go_noneflag>=3)
			{
				go_noneflag=0;
				car.go_none=1;//标志前面没线了
			}
		}
		go_haveflag=0;//清零，不是连续的			
		return -1;
	}
	else//找到值
	{
		go_noneflag=0;
		if(car.state==7)
		{
			go_haveflag++;		
			if(go_haveflag>=3)
			{
				go_haveflag=0;
			}
		}
		car.buttom = (car.all_av*num-pill)/num;
		return find;
	}
}

//自动曝光
int auto_Exposure(void)
{

		if(car.auto_enable)
		{
				if(car.minav2-35 <= car.auto_av-3)
				{
					if(car.delay_time < car.auto_max)
					{
						car.delay_time++;
					}
				}
				else if(car.minav2-35 >= car.auto_av+3)
				{
					if(car.delay_time > car.auto_min)
					{
						car.delay_time--;
					}
				}
		}
		return car.delay_time;
}

void PIDCCD(float line,float P,float D)//输入找到的中线//模式0，1，2。。。。 
{
	car.line_11=car.line_l;
	car.line_l=car.line_n;
	car.line_n=line;
	
//Real值//就是滤波后的值
//	car.line_R_1l=car.line_R_1;//左黑线
//	car.line_R_1=car.line_R;
//	car.line_R=line*0.7f+car.line_R_1*0.2f+car.line_R_1l*0.1f;//一阶互补滤波
	car.line_R=line;
//line -> CCD_PID.line_R
//开始PID
	car.ERROR1_l=car.ERROR1;//存储上次的变化量
	car.ERROR1=(car.line_R-car.AV);//平时是主要矛盾//黑线
	
	car.P1OUT=P*car.ERROR1;
	car.D1OUT=D*(car.ERROR1-car.ERROR1_l);
	car.OUT=car.P1OUT+car.D1OUT;

//-----------------------------------------一样
//防止超控//可修改
	if(car.OUT>0.5f)
		car.OUT=0.5f;
	else if(car.OUT<-0.5f)
		car.OUT=-0.5f;
}

