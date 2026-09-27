#ifndef __MYUSART_H__
#define __MYUSART_H__
#include "sys.h"
#include <stdio.h>
#define u8   uint8_t
#define u32  uint32_t
#define CAMERA_DATA_LEN  3	//新视觉协议：0xAA + ASCII '0'~'5' + 0xBB



void SYN_FrameInfo(u8 Music, u8 *HZdata);
void SYN_ZL(uint8_t id);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
HAL_StatusTypeDef USART2_StartRead(void);
void USART2_StopRead(void);
void ZL_record(uint8_t id);
void USART2_Test(void);
uint8_t majorityVote(uint8_t *dataArray, uint16_t length);	//多数表决函数，统计出识别次数最多的数字
int fputc(int ch, FILE *f);
#endif
