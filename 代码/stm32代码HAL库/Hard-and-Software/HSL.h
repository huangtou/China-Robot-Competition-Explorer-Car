#ifndef __HSL_H
#define __HSL_H

#include "sys.h"

#define HSDA_IN()  {GPIOD->MODER&=~(3<<(2*2));GPIOD->MODER|=0<<2*2;}	
#define HSDA_OUT() {GPIOD->MODER&=~(3<<(2*2));GPIOD->MODER|=1<<2*2;} 

#define I2C_SCL_PIN  GPIO_PIN_3  // SCL 
#define I2C_SDA_PIN  GPIO_PIN_2  // SDA 
#define I2C_GPIO_PORT GPIOD      // GPIO 

#define I2C_SCL_HIGH() HAL_GPIO_WritePin(I2C_GPIO_PORT,I2C_SCL_PIN,GPIO_PIN_SET)
#define I2C_SCL_LOW()  HAL_GPIO_WritePin(I2C_GPIO_PORT,I2C_SCL_PIN,GPIO_PIN_RESET)

#define I2C_SDA_HIGH() HAL_GPIO_WritePin(I2C_GPIO_PORT,I2C_SDA_PIN,GPIO_PIN_SET)
#define I2C_SDA_LOW()  HAL_GPIO_WritePin(I2C_GPIO_PORT,I2C_SDA_PIN,GPIO_PIN_RESET)

#define I2C_SDA_READ() HAL_GPIO_ReadPin(I2C_GPIO_PORT,I2C_SDA_PIN)

void HSL_Init(void);
void I2C_Delay(void);
void I2C_Stop(void);
void I2C_Start(void);
void I2C_SendBit(uint8_t bit);
uint8_t I2C_ReadBit(void);
void I2C_SendByte(uint8_t byte);
uint8_t I2C_ReadByte(uint8_t ack);
void I2C_Read_Sensor(uint8_t *recv_value);
void HSL_Read_Sensor(uint8_t *recv_value);
int HSL(void);
void ERR_Read(uint8_t *recv_value);


#endif
