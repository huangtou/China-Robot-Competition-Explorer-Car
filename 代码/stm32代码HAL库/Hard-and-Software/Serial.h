#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "main.h"
#include <stdio.h>

int fputc(int ch, FILE *f);

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

typedef struct
{
    HAL_StatusTypeDef (*Vofa)(const char *fmt, ...);
    HAL_StatusTypeDef (*HMI)(const char *fmt, ...);
} Serial_TypeDef;

extern Serial_TypeDef Serial;

#endif
