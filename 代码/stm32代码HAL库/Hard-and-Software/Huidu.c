#include "sys.h"


static volatile uint16_t DMA_Data[12]={0};

#define THRESHOLD 3000  //灰度判定白线的阈值
#define NUM_SENSORS 12  //灰度传感器数量

uint16_t AD(uint8_t num)	//函数功能：读取指定灰度数据的函数//返回值：AD_data[num]指定灰度检测到的模拟量//形参：num，可以任意指定的灰度编号
{
	if(num<12)
	{
		AD_data[num]=AD_value[num];		//通过 定义AD_value数组 以及 使用HAL_ADC_Start_DMA(&hadc3,(uint32_t*)AD_value,12);（用于将数据存入AD_value数组） 完成灰度数据读取
		return AD_data[num];
	}
	else return 0;
}

float Huidu_findline(uint16_t* AD_data, int16_t Line_Search_temp) //在move.c的huidu_ccd_pid函数中使用//函数功能：灰度寻找白线的实际位置//返回值：line = car.ADC_value白线的实际位置//形参：AD_data所有灰度的模拟量，Line_Search_temp左中右巡模式
{
    float line = -1.0f; //白线实际的位置
    int ADC_Lost = 1;   //灰度丢失标志位
    int count;					//计数用的
		
    if (Line_Search_temp == -1 || Line_Search_temp == 1) 							//左巡线或右巡线
		{																																	
        int start = (Line_Search_temp == -1) ? 0 : NUM_SENSORS - 1;		//如果是左巡线模式，起始位置从0开始，向右遍历；如果是右巡线模式，起始位置从11开始，向左遍历
        int end = (Line_Search_temp == -1) ? NUM_SENSORS : -1;				//如果是左巡线模式，结束位置为12；如果是右巡线模式，结束位置为-1
        int step = (Line_Search_temp == -1) ? 1 : -1;									//如果是左巡线模式，依次+1，实现从0到12遍历；如果是右巡线模式，依次-1，实现从11到-1遍历

        for (count = start; count != end; count += step) 							//for(i=0; i!=12; i++)/for(i=11; i!=-1; i--)
				{
            if (AD_data[count] > THRESHOLD) 													//如果有灰度找到白线（THRESHOLD为白线检测阈值3000）//判断一下具体哪个灰度检测到的
						{
                if ((count + step >= 0 && count + step < NUM_SENSORS) && AD_data[count + step] > THRESHOLD) //有两个灰度扫到白线的情况
								{
										car.ADC_value = count + 0.5f * step;							//取这两个灰度的中值为实际白线的位置
								} 
								else 																																												//只有一个灰度扫到白线的情况
								{
										car.ADC_value = count + 0.0f; 										//实际白线的位置就是灰度的编号
								}
                ADC_Lost = 0;																					//”灰度丢失标志位”置0，即检测到白线
                break;
            }
        }
    }
		else if (Line_Search_temp == 0) 																	//中巡线
		{
				//寻找一个检测到白线的灰度
        for (count = NUM_SENSORS / 2; count >= 0; count--) 						//for(i=6; i>=0; i--)从中间向左遍历[6]~[0]7个灰度 //NUM_SENSORS 灰度个数12，count=6
				{
            if (AD_data[count] > THRESHOLD) 													//count = 6~0//从中间向左遍历灰度是否有扫到白线的 //THRESHOLD灰度阈值3000，
						{
                car.ADC_value = count;																//car.ADC_value = 6~0 //记录扫到白线的灰度编号
                ADC_Lost = 0;																					//”灰度丢失标志位”置0，即检测到白线
                break;																								//只要有一个灰度检测到白线就停止遍历
            }
						else if (AD_data[NUM_SENSORS - 1 - count] > THRESHOLD) 		//count = 5~11//从中间向右遍历灰度是否有扫到白线的
						{
                car.ADC_value = NUM_SENSORS - 1 - count;							//car.ADC_value = 7~11//记录扫到白线的灰度编号
                ADC_Lost = 0;																					//”灰度丢失标志位”置0，即检测到白线
                break;
            }
        }
				//再找到一个相邻的检测到白线的灰度，算他俩中值，这个中值就是实际白线的位置
        if (ADC_Lost == 0) 																						//如果”灰度丢失标志位”==0，即检测到白线
				{
            int line_int = (int)car.ADC_value;												//将灰度编号赋给line_int （0~11都能取到）
            if ((line_int + 1 < NUM_SENSORS) && AD_data[line_int + 1] > THRESHOLD)	//line_int + 1 < NUM_SENSORS -> 0~10，前10个灰度编号检测到白线的情况//并且比灰度编号大一个的灰度也检测到白线了
						{
                car.ADC_value += 0.5f;	//相当于取 灰度编号 和 比灰度编号大一个的灰度 的中值//比如说car.ADC_value==3，即第四个灰度检测到白线；且AD_data[3 + 1] > THRESHOLD，即第五个灰度也检测到白线。那么实际的白线位置就取他俩的中值(3+4)/2=3.5
            }
						else if ((line_int - 1 >= 0) && AD_data[line_int - 1] > THRESHOLD) 			//line_int - 1 >= 0 -> 0~10，前10个灰度编号检测到白线的情况//并且比灰度编号小一个的灰度也检测到白线了
						{
                car.ADC_value -= 0.5f;	//同理，取他俩中值
            }
        }
    }

    if (!ADC_Lost) 																										//如果检测到白线
		{
        line = car.ADC_value;																					//把灰度编号赋给”实际白线位置”
    }
    return line;																											//返回”实际白线位置”
}
