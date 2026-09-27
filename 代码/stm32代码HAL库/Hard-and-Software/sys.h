#ifndef __sys_H__
#define __sys_H__

extern int flagroad;

#include "stm32f4xx_hal.h"
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"


/////////////////
#include "myusart.h"
#include "timer.h"
#include "huidu.h"
#include "key.h"
#include "GD.h"
#include "HWT101.h"
#include "HSL.h"
#include "CCD.h"
#include "MOVE.h"
#include "base.h"
#include "private.h"
#include "DJ.h"
#include "Kalman.h"
#include "Serial.h"
/////////////////
#include "stdio.h"
#include "stdint.h"
#include "math.h"
#include "string.h"
#include "stdlib.h"

////////////////

typedef struct	//car的结构体，在sys.h 188中声明了全局变量
{
///////////////////灰度部分
	float Av;   //中间灰度的值  	PID.Error=PID.Av - PID.ADC_value;
	
	float ADC_value;//灰度值
	u32 ADnum;
	
	
	float Error;  //灰度巡线的误差
	float Last_Error;  //上一个误差，PID可用
	
	
	float huidu_P;
	float huidu_I;
	float huidu_D;
	
	//设置两套PID	PID_change()   速度小于400用PID1   速度大于400用PID2
	
	float huidu_P1;   
	float huidu_I1;
	float huidu_D1;

	float huidu_P2;
	float huidu_I2;
	float huidu_D2;
	
	float huidu_P3;
	float huidu_I3;
	float huidu_D3;
	
	float huidu_P4;
	float huidu_I4;
	float huidu_D4;
	
	float huidu_P5;
	float huidu_I5;
	float huidu_D5;
	
	float Pout;
	float Iout;
	float Dout;

	
///////////////////编码器
	u8 enco;
	volatile int32_t enco_counter;
	int32_t Target_dis;
	volatile float Target;//目标速度		
	
	
	
///////////////////陀螺仪部分	
	volatile int yaw;//角度数
	volatile int yaw_cl;
	volatile int yaw_1;//角度数
	volatile int yaw_2;//角度数
	volatile int yaw_3;//角度数
	volatile int yaw_4;//角度数
	volatile int yaw_5;//角度数
	volatile int yaw_6;//角度数
	volatile int yaw_7;//角度数
	volatile int yaw_8;//角度数
	volatile int yaw_9;//角度数
	volatile int yaw_10;//角度数
	volatile int yaw_11;//角度数
	volatile int yaw_12;//角度数
	volatile int yaw_13;//角度数
	volatile int yaw_14;//角度数
	volatile int yaw_15;//角度数
	volatile int yaw_16;//角度数
	volatile int yaw_17;//角度数
	volatile int yaw_18;//角度数
	volatile int yaw_19;//角度数
	volatile int yaw_20;//角度数
	volatile float Angle;//角度
	int yaw_ca;//角度数标定
	float Target_Angle;
	float Target_Angle_1;
    u8 HWT101_getOnce;

	
//////////////////////CCD部分
	uint16_t all_av;
    uint16_t zuo_av;
    uint16_t you_av;
    uint16_t max;
    uint16_t min;
	uint8_t minpixel[10];
    float   minav;
	uint8_t minpixel2[10];
    float   minav2;
	float bili;
	uint16_t bound;
	float find_left;
	float find_mid;
	float find_right;
	float AV;//目标值
	float AV2;//目标值
	u8 way4;
	u8 go_find;
	int8_t go_none;//没线标志
	u8 buttom;//底部平均值
	
	float line_n,line_l,line_11;//line_now,line_last,line_last_last
	float line_R,line_R_1,line_R_1l;//滤波后的值


	float CCD_P1;   
	float CCD_I1;
	float CCD_D1;

	float CCD_P2;
	float CCD_I2;
	float CCD_D2;

/////ccd输出
	float ERROR1;
	float ERROR1_l;
	float P1OUT;
	float D1OUT;
	
	
	//自适应曝光参数
	u8 auto_enable;//自动曝光使能位
	int auto_min;//delay_time最小
	int auto_max;//delay_time最大
	int auto_av;//给定曝光量
	int delay_time;//曝光时间	
	
	
//////////////////控制部分
	int8_t Direct;//车辆方向
	int8_t state;	
	int8_t Line_Search;
	int8_t pid_able;
	int8_t ccd_use;
	int8_t huidu_use;
	int8_t line;
	float find;	
	float OUT;
	int time;
	int gd_id;
	u8 adjust;
	
	
	
	int (*callBack)(void);//回调函数
}CAR_STRUCT;


/////////////////

extern CAR_STRUCT car;
extern Kalman kfp;
extern volatile PID pid_1;
extern volatile PID pid_2;
extern volatile PID pid_3;
extern volatile PID pid_4;
extern volatile PID pid_F;
extern volatile PID pid_A;
extern volatile PID pid_R;

extern volatile int velocity[5];
extern volatile int32_t encoder[5];
extern unsigned char HSL_value[3];
extern uint8_t Pixel[128];
extern 	uint32_t CHANNEL_CCD;
extern	int cou;
extern uint16_t AD_value[12];
extern uint16_t AD_data[12];
extern int8_t flag6times;
extern int ADW[30];
extern Kalman  kfp;
extern volatile uint8_t USART2_RecieveData[CAMERA_DATA_LEN];
extern volatile uint8_t usart2_flag;
extern volatile uint8_t ZLflag;
extern volatile uint8_t ZL_ID;
extern volatile uint8_t ZL[5];

/////////////////

void Para_Init(void);
void Display(void);
void Delay_us(uint32_t nus);
void Delay_ms(uint32_t nms);

/////////////////
#define LIMIT_MIN_MAX(x,min,max) (x) = (((x)<=(min))?(min):(((x)>=(max))?(max):(x))) //积分限幅函数	
#define u8   uint8_t
#define u16   uint16_t
#define u32  uint32_t
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 

//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入


#endif

