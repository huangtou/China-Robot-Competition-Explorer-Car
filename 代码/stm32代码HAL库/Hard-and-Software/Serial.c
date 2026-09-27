#include "Serial.h"

#include <stdarg.h>

#define ReceiveBufferSize 128

#define SERIAL_BUF_SIZE 128
#define SERIAL_TIMEOUT  100

uint8_t ReceiveData[ReceiveBufferSize];

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

extern UART_HandleTypeDef huart3;



static HAL_StatusTypeDef Serial_Vofa(const char *fmt, ...)
{
    char buf[SERIAL_BUF_SIZE];
    int len;
    va_list args;

    va_start(args, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    if (len < 0 || len >= SERIAL_BUF_SIZE - 2)
    {
        return HAL_ERROR;
    }

    // VOFA+ FireWater 一帧以换行结束
    buf[len++] = '\n';

    return HAL_UART_Transmit(&huart2, (uint8_t *)buf, len, 100);
}

static HAL_StatusTypeDef Serial_HMI(const char *fmt, ...)
{
    char buf[SERIAL_BUF_SIZE];
    int len;
    va_list args;

    va_start(args, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    if (len < 0 || len >= SERIAL_BUF_SIZE - 4)
    {
        return HAL_ERROR;
    }

    // TJC 串口屏结束符
    buf[len++] = 0xFF;
    buf[len++] = 0xFF;
    buf[len++] = 0xFF;

    return HAL_UART_Transmit(&huart1, (uint8_t *)buf, len, 100);
}

Serial_TypeDef Serial =
{
    .Vofa = Serial_Vofa,
    .HMI  = Serial_HMI
};
