#include "sys.h"

volatile uint8_t USART2_RecieveData[CAMERA_DATA_LEN];	//接收缓冲区

volatile uint8_t usart2_flag;//用于中断开启标志位
volatile uint8_t ZLflag = 0;//用于判断有无数据传回
volatile uint8_t ZL_ID = 0;
volatile uint8_t ZL[5] = {0};//用于记录第一次直立景点的数字，方便第二次播报

//usart1重定向
int fputc(int ch, FILE *f)
{  
  while((USART1->SR & 0X40) == RESET);
	
	USART1->DR = (uint8_t)ch;  
	
  return ch;
}

//串口三语音模块

//Music:选择背景音乐。0:无背景音乐，1~15：选择背景音乐
//*HZdata:播报语音内容。[v]音量大小:0~16
void SYN_FrameInfo(u8 Music, u8 *HZdata)
{
  /****************需要发送的文本**********************************/
  unsigned  char  Frame_Info[50];
  unsigned  char  HZ_Length;
  unsigned  char  ecc  = 0;  			//定义校验字节
  unsigned  int i = 0;
  HZ_Length = strlen((char*)HZdata); 			//需要发送文本的长度

  /*****************帧固定配置信息**************************************/
  Frame_Info[0] = 0xFD ; 			//构造帧头FD
  Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
  Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
  Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令
  Frame_Info[4] = 0x01 | Music << 4 ; //构造命令参数：背景音乐设定

  /*******************校验码计算***************************************/
  for(i = 0; i < 5; i++)   				//依次发送构造好的5个帧头字节
  {
    ecc = ecc ^ (Frame_Info[i]);		//对发送的字节进行异或校验
  }

  for(i = 0; i < HZ_Length; i++)   		//依次发送待合成的文本数据
  {
    ecc = ecc ^ (HZdata[i]); 				//对发送的字节进行异或校验
  }
  /*******************发送帧信息***************************************/
  memcpy(&Frame_Info[5], HZdata, HZ_Length);
  Frame_Info[5 + HZ_Length] = ecc;
  
  HAL_UART_Transmit(&huart3, Frame_Info, 5 + HZ_Length + 1, 100000);
  
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)		//串口接收回调函数
{
	if(huart==&huart2 && usart2_flag)		//先判断传进来的数据是否来自huart2
	{
		if(USART2_RecieveData[0] == 0xAA && USART2_RecieveData[2] == 0xBB)
		{
			ZL_ID = USART2_RecieveData[1] - '0';
			if(ZL_ID > 0 && ZL_ID < 6)
			{
				
				ZLflag = 1;
				usart2_flag = 0;
				return;
			}
		}

		/* 滑动保留最近两个字节，使接收从帧中间开始时也能重新对齐 */
		USART2_RecieveData[0] = USART2_RecieveData[1];
		USART2_RecieveData[1] = USART2_RecieveData[2];
		if(HAL_UART_Receive_IT(&huart2,(uint8_t *)&USART2_RecieveData[2],1) != HAL_OK)
		{
			usart2_flag = 0;
		}
	}
}

HAL_StatusTypeDef USART2_StartRead(void)
{
	HAL_StatusTypeDef status;

	usart2_flag = 0;
    ZLflag = 0;
    ZL_ID = 0;
	//DJ_down();
	
	status = HAL_UART_AbortReceive(&huart2);
	if(status != HAL_OK)
	{
		return status;
	}

	__HAL_UART_CLEAR_OREFLAG(&huart2);
	
	USART2_RecieveData[0] = 0;
	USART2_RecieveData[1] = 0;
	USART2_RecieveData[2] = 0;
	
	usart2_flag = 1;
	
	status = HAL_UART_Receive_IT(&huart2,(uint8_t *)&USART2_RecieveData[2],1);
	if(status != HAL_OK)
	{
		usart2_flag = 0;
	}

	return status;
}

void USART2_StopRead(void)
{
	usart2_flag = 0;
	HAL_UART_AbortReceive(&huart2);
}

void SYN_ZL(uint8_t id)
{
	switch (id)
	{
		case 1: SYN_FrameInfo(0, (uint8_t *)"[v13][t5]到达东岳泰山");break;
		case 2: SYN_FrameInfo(0, (uint8_t *)"[v13][t5]到达西岳华山");break;
		case 3: SYN_FrameInfo(0, (uint8_t *)"[v13][t5]到达南岳衡山");break;
		case 4:	SYN_FrameInfo(0, (uint8_t *)"[v13][t5]到达北岳恒山");break;
		case 5: SYN_FrameInfo(0, (uint8_t *)"[v13][t5]到达中岳嵩山");break;
		default:SYN_FrameInfo(0, (uint8_t *)"[v13][t5]到达直立景点");break;
	}
}

void ZL_record(uint8_t id)
{
	if(id < 1 || id > 5) return;
	ZL[id - 1] = ZL_ID;
	ZL_ID = 0;
	ZLflag = 0;
}

void USART2_Test(void)
{
	while(1)
	{
		if(USART2_StartRead() == HAL_OK)
		{
			uint32_t usart2_start_tick = HAL_GetTick();
			while(usart2_flag == 1 && (HAL_GetTick() - usart2_start_tick) < TIME_READ)
			{
				HAL_Delay(1);
			}

			USART2_StopRead();
			if(ZLflag)
			{
				SYN_ZL(ZL_ID);
			}
		}
		HAL_Delay(1000);
	}
}

uint8_t majorityVote(uint8_t *dataArray, uint16_t length)	//多数表决函数，统计出识别次数最多的数字
{
	uint16_t count[6] = {0};  //初始化计数器，假设数据范围是1-5//索引0不用，1-5对应数据1-5
	
	for(uint16_t i = 0; i < length; i++) //统计每个数字出现的次数
	{
		if(dataArray[i] >= 1 && dataArray[i] <= 5) //确保数据在有效范围内
		{
			count[dataArray[i]]++;
		}
	}
	
	uint8_t maxNumber = 1;//找到出现次数最多的数字
	uint16_t maxCount = 0;
	
	for(uint8_t i = 1; i <= 5; i++) 
	{
		if(count[i] > maxCount) 
		{
			maxCount = count[i];
			maxNumber = i;
		}
	}
	return maxNumber;
}
