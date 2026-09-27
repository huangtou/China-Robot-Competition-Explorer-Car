#include "sys.h"

void I2C_Delay(void)
{
    for (volatile int i = 0; i <200; i++);
}

void I2C_Start(void)
{
    I2C_SDA_HIGH();
    I2C_SCL_HIGH();
    I2C_Delay();
    I2C_SDA_LOW();  // SDA 拉低，开始信号
    I2C_Delay();
    I2C_SCL_LOW();  // SCL 拉低
}

void I2C_Stop(void)
{
    I2C_SDA_LOW();
    I2C_SCL_HIGH();
    I2C_Delay();
    I2C_SDA_HIGH();  // SDA 拉高，停止信号
    I2C_Delay();
}

void I2C_SendBit(uint8_t bit)
{
    if (bit)
        I2C_SDA_HIGH();
    else
        I2C_SDA_LOW();
    
    I2C_SCL_HIGH();
    I2C_Delay();
    I2C_SCL_LOW();
    I2C_Delay();
}

uint8_t I2C_ReadBit(void)
{
    uint8_t bit;
    I2C_SDA_HIGH();  // 释放 SDA 以便读取数据
    I2C_SCL_HIGH();
    I2C_Delay();
    bit = I2C_SDA_READ();
    I2C_SCL_LOW();
    I2C_Delay();
    return bit;
}

void I2C_SendByte(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        I2C_SendBit((byte & 0x80) != 0);  // 发送最高位
        byte <<= 1;
    }
    // 接收应答位
    I2C_ReadBit();
}

uint8_t I2C_ReadByte(uint8_t ack)
{
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++)
    {
        byte <<= 1;
        byte |= I2C_ReadBit();  // 读取每一位
    }
    // 发送应答位
    I2C_SendBit(ack ? 0 : 1);  // 发送 ACK 或 NACK
    return byte;
}

void HSL_Read_Sensor(uint8_t *recv_value)
{
    I2C_Start();  // 发送起始信号

    I2C_SendByte(0x9E);  // 发送从机地址（写模式）
    I2C_SendByte(0xD1);       // 发送命令 0xD0
    I2C_Stop();               // 停止信号

    I2C_Delay();  // 等待一小段时间

    I2C_Start();  // 重新启动，进入读模式
    I2C_SendByte(0x9F);  // 发送从机地址（读模式）


    // 接收数据
    recv_value[0] = I2C_ReadByte(1);  // 读第1个字节，发送 ACK
    recv_value[1] = I2C_ReadByte(1);  // 读第2个字节，发送 ACK
    recv_value[2] = I2C_ReadByte(0);  // 读第3个字节，发送 NACK
//	printf("%d\n",HSL_value[0]);
    I2C_Stop();  // 停止信号
}

void I2C_Read_Sensor(uint8_t *recv_value)
{
    I2C_Start();  // 发送起始信号

    I2C_SendByte(0x9E);  // 发送从机地址（写模式）
    I2C_SendByte(0xAA);       // 发送命令 0xD0
    I2C_Stop();               // 停止信号

    I2C_Delay();  // 等待一小段时间

    I2C_Start();  // 重新启动，进入读模式
    I2C_SendByte(0x9F);  // 发送从机地址（读模式）

	
    // 接收数据
    recv_value[2] = I2C_ReadByte(0);  // 读第1个字节，发送 ACK

    I2C_Stop();  // 停止信号
}

void ERR_Read(uint8_t *recv_value)
{
    I2C_Start();  // 发送起始信号

    I2C_SendByte(0x9E);  // 发送从机地址（写模式）
    I2C_SendByte(0xDE);       // 发送命令 0xD0
    I2C_Stop();               // 停止信号

    I2C_Delay();  // 等待一小段时间

    I2C_Start();  // 重新启动，进入读模式
    I2C_SendByte(0x9F);  // 发送从机地址（读模式）

	
    // 接收数据
    recv_value[2] = I2C_ReadByte(0);  // 读第3个字节，发送 NACK

    I2C_Stop();  // 停止信号
}

int HSL(void)
{
	int ic=0;
	for(int is=0;is<20;is++)
	{
		HSL_Read_Sensor(HSL_value);
		if (HSL_value[0]>50 && HSL_value[0]<90) ic++;
		Delay_ms(1);
	}
	if (ic>=3) return 1;
	else return 0;
	
}

//100
//int HSL(void)
//{
//	int ic=0;
//	for(int is=0;is<10;is++)
//	{
//		HSL_Read_Sensor(HSL_value);
//		if (HSL_value[0]>140 & HSL_value[0]<175 ) ic++;
//	}
//	if (ic>=5) return 1;
//	else return 0;
//	
//}

