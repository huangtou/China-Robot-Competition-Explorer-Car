#ifndef __CCD_H__
#define __CCD_H__
#include "sys.h"

#define Time_1 10
#define Time_2 120

#define CCD2_CHANNEL ADC_CHANNEL_4
#define CCD1_CHANNEL ADC_CHANNEL_5

#define CLK_out PGout(0)
#define SI_out  PGout(1)

#define CLK_out_2 PEout(7)
#define SI_out_2  PEout(8)

void delayus(u32 m);
uint32_t ME_POW(uint32_t j,uint32_t n);
uint8_t Convert_ADC_to_8bit(uint32_t adcValue, uint32_t adcResolution);
uint8_t Read_ADC_data(ADC_HandleTypeDef* hadc, uint32_t Channel) ;///改的这
void Read_128(uint32_t Channel);
uint32_t Gap_Avr(uint8_t* pixel,int gap);
uint32_t AvEasy(uint8_t *pixel,int begin,int end);//简单求取平均值
uint16_t Aver128(uint8_t* pixel, uint8_t begin, uint8_t end);
int aboutEqual(float a,float b,float c);//约等于
float CcdFindLine2(uint8_t *pixel,const int num,int fly,int8_t way);
int auto_Exposure(void);
void PIDCCD(float line,float P,float D);
#endif

