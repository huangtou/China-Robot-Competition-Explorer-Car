/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

	CAR_STRUCT car;
	Kalman     kfp;
	volatile PID pid_1;
	volatile PID pid_2;
	volatile PID pid_3;
	volatile PID pid_4;
	
	volatile PID pid_F;//    
	volatile PID pid_A;//
	volatile PID pid_R;// 
	
//	volatile float I_limit=100;					
	volatile int A_limit=180;						
	volatile int P_V_limit=900;
	
	
	volatile int velocity[5]={0};
	volatile int32_t encoder[5]={0};
	uint16_t AD_value[12];
	uint16_t AD_data[12];
	unsigned char HSL_value[3];
	u8 Function_Value=0;
	int cou=0;
	uint32_t CHANNEL_CCD;
	int8_t flag6times;
	int carrr;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
int count;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_ADC3_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM8_Init();
  MX_TIM9_Init();
  MX_TIM12_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM10_Init();
  MX_ADC2_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  
	HAL_TIM_Base_Start_IT(&htim7);//read
	HAL_ADC_Start_DMA(&hadc3,(uint32_t*)AD_value,12);
	move_init();
	Para_Init();
	Kalman_Init();
	Dj_all_Init();
	printf("\x01\xff\xff\xff");
	printf("t0.txt=\"no\"\xFF\xFF\xFF");
	printf("t13.txt=\"NULL\"\xFF\xFF\xFF");
//	HAL_UART_Receive_IT(&huart2, &usart2_dataRcvd, 1);
	HAL_Delay(50);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

//	SYN_FrameInfo(0, "[v16][t5]?  ");
//	HAL_TIM_Base_Start_IT(&htim6);
   Function_Value = Function_Mode();
//	car.state = 1;
  HAL_Delay(50);
   
//	car.Target=250;
//  HWT101_Calibrate();
//  HWT101_go(0,100,NULL);
//    go((1<<0)|(1<<1),100,0,0,NULL);******************
//	move_control(800,0,0,0);
//	go_time(100000,-100,1,0,NULL);
//	go_enco(50,100,0,0,NULL);
//	HWT101_Calibrate();
//	HWT101_go(0,0,NULL);
//	car.Target=-100;
//	car.state=100;
//while(1)	
//{
//	HAL_TIM_Base_Stop_IT(&htim6);
//	move_control(0,0,0,0);
//	HAL_Delay(5000);
//	HAL_TIM_Base_Start_IT(&htim6);
//	HAL_Delay(10000);
//}
  
  while (1)
  {
	  if(Function_Value==5)		//ALL      
		{
			DJ_QQB();
//			DJ_down();
			USART2_StartRead();
			HAL_TIM_Base_Stop_IT(&htim7);
			Display();
		}
		if(Function_Value==1)
		{
			Ready();
			part1_pro();
			part2_pro();
			part3_pro();
			part4_pro();
			Ready();
			part1_pro();
			part2_pro();
			part3_pro();
			part4_pro();

			
			stop(1);
			while(1);
		}
		if(Function_Value==2)
		{
			
			Ready();
			
			
			part1_pro();
			part2_pro();
			part3_pro();
			part4_pro();
			stop(1);
			while(1);
		}
		
		if(Function_Value==3)	//huidu_PID_test//zuo_you_test
		{
			Ready();
			
			stop(20);


			
//			DJ_QQB();
////			DJ_down();
//			stop(500);

////////////////////////////////////////////			
//			qi_bu();
//			car.Target_dis = 100;
//			go_acc(80,90,110,1,0,my_enco);
//			
////			car.Av = 8.5;	//change huidu_av
//			
//			go_enco(400,110,1,0,NULL);
//			
//			car.Target_dis = 50;
//			go_acc(80,110,90,1,0,my_enco);
////////////////////////////////////////////
//			
////			car.Target_dis = 55;
////			go_acc(50,85,110,0,1,my_enco);
////			go_GD(1,110,0,1,NULL);						//到阶梯
////			
////			go_enco(80,80,0,0,NULL);		//过阶梯/////////////////////////////////////////////////
////			go_enco(35,90,0,0,NULL);		//过阶梯/////////////////////////////////////////////////

////			go_enco(100,110,0,-1,NULL);		//到直线

////			go_acc(100,110,300,0,0,my_huidu1011);
////			go_enco(40,300,0,0,NULL);					//测  场地实际距离170cm，去掉减速距离，走的距离要比170cm少一点
////			go_acc(100,300,110,0,-1,my_huidu1011);

////			
////			go_enco(70,110,0,-1,NULL);		//左巡进岔路///////////////////////////////////////////

////			car.Target_dis = 30;
////			go_acc(100,110,250,0,0,my_enco);

////			car.Target_dis = 90;
////			go_acc(100,250,85,0,0,my_enco);
////			
////			go_enco(40,60,0,0,my_qc1);
			
			while(1){}
		}
		
		if(Function_Value==4)		  //test HWT101_GD
		{
			DJ_down();
			stop(500);
			
			HWT101_Calibrate();
			HWT101_GD(0,100,0,0,NULL);

			while(1){}
		}
		
		if(Function_Value==8)		  //test HWT101_GD
		{
			Ready();
			go_acc(100,-90,-100,0,0,NULL);
			
			while(1){}
		}
		
		
		if(Function_Value==6)		  
		{		
			

			DJ_camera_look();
			stop(500);
				
			
	
//			TIM9->CCR1=250;
//			stop(200);
//			TIM9->CCR1=450;
//			stop(200);
//			TIM9->CCR1=250;
			
			while(1)
			{
				USART2_StartRead();
				
				stop_read(TIME_READ);
				
				USART2_StopRead();
				SYN_ZL(ZL_ID);
				
				HAL_Delay(5000);
			}
		}



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

//			  Read_128(ADC_CHANNEL_4);
//			  Gap_Avr(Pixel, 4);
//			  Aver128(Pixel,0,31);
//			  for (int i=0;i<32;i++)
//			  {
//				  printf("%d,%d,%f\n",Pixel[i],car.delay_time,car.minav2);
//			  }
//			  for (int i=0;i<20;i++)
//			  {	
//				  printf("%d,%d,%f\n",225,car.delay_time,car.minav2);
//			  
//			  }
//			  auto_Exposure();
//			  HAL_Delay(car.delay_time);

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */







