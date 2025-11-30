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
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "fdcan.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include <string.h>
#include "FOC.h"
#include "FOC_Portable.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// typedef struct{
//   float error;
//   float error_int;
//   float kp;
//   float ki 
// }
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define FLOAT_NUM 5//18
#define TAIL_INDEX FLOAT_NUM*4
#define TX_NUM TAIL_INDEX+4


#define UART2_FLOAT_NUM 7
#define UART2_FLOAT_TAIL_INDEX UART2_FLOAT_NUM*4
#define UART2_TX_NUM UART2_FLOAT_TAIL_INDEX+4


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// uint8_t tx_buf[TX_NUM]={0};
// uint8_t LED_array[25]={	55,55,55,55,133,133,133,133,
// 												133,133,55,55,133,133,133,133,
// 												55,55,55,55,133,133,133,133,0};

// float etheta = 0.0f;
// float etheta_angle = 0.0f;

// float VBarbus = 12.0f;
// float Ud = 0.0f,Uq = 0.0f;
// float Ualpha = 0.0f,Ubeta = 0.0f;
// float PWM_cmp[3] = {0.0f, 0.0f, 0.0f};

// float glb_Ua = 0.0f,glb_Ub = 0.0f,glb_Uc = 0.0f;

// float glb_section = 0.0f;

// float glb_vx = 0.0f, glb_vy = 0.0f, glb_vz = 0.0f;

// float glb_t0 = 0.0f, glb_t1 = 0.0f, glb_t2 = 0.0f;

// float last_sample_current[3]={0.0f,0.0f,0.0f};
// float sample_cureent[3]={0.0f,0.0f,0.0f};
// uint8_t uart2_buf[UART2_TX_NUM]={0.0f};


// uint8_t start_motor_flag =0; 

// float filtered_current[3] = {0.0f};

// float sliding_window[3][SLIDING_WINDOW_SIZE] = {0.0};
// float sort_temp[SLIDING_WINDOW_SIZE] = {0.0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // if(htim->Instance==TIM6)
  // {
  //   etheta += 0.1000f;
	// 	if(etheta>6.28f)
	// 		etheta =0.0f;

  //   etheta_angle= etheta / 3.1415926 *180;
  // }
}






// void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
// {
//   if(hadc->Instance==ADC1)
//   {
//     sample_cureent[0] = (float)HAL_ADCEx_InjectedGetValue(hadc,ADC_INJECTED_RANK_1)/4095.0f*3.3f-1.65;
// 		sample_cureent[1] = (float)HAL_ADCEx_InjectedGetValue(hadc,ADC_INJECTED_RANK_2)/4095.0f*3.3f-1.65;
//     sample_cureent[2] = (float)HAL_ADCEx_InjectedGetValue(hadc,ADC_INJECTED_RANK_3)/4095.0f*3.3f-1.65;
//     filtered_current[0] = sample_cureent[0]*0.9f+filtered_current[0]*0.1f;
//     filtered_current[1] = sample_cureent[1]*0.9f+filtered_current[1]*0.1f;
//     filtered_current[2] = sample_cureent[2]*0.9f+filtered_current[2]*0.1f;
//     memcpy(uart2_buf,&glb_section,4);
//     memcpy(uart2_buf+4,(uint8_t*)&sample_cureent,12);
//     memcpy(uart2_buf+16,(uint8_t*)&filtered_current,12);
//     HAL_UART_Transmit_DMA(&huart2,uart2_buf,UART2_TX_NUM);

//   }
// }


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	 //Flash_OB_Configure();
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
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_FDCAN1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_UART4_Init();
  MX_USART2_UART_Init();
  MX_ADC2_Init();
  MX_TIM6_Init();
  MX_TIM8_Init();
  MX_TIM15_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */
	

//HAL_TIM_Base_Start_IT(&htim6);

// Phase_Current_Sample_Start();
// LIVE_Light_Off();

//   Uq = 1.0  ; 
// //	
// 	tx_buf[TAIL_INDEX] = 0x00;
// 	tx_buf[TAIL_INDEX+1] = 0x00;
// 	tx_buf[TAIL_INDEX+2] = 0x80;
// 	tx_buf[TAIL_INDEX+3] = 0x7f;

//   uart2_buf[UART2_FLOAT_TAIL_INDEX] = 0x00;
//   uart2_buf[UART2_FLOAT_TAIL_INDEX+1] = 0x00;
//   uart2_buf[UART2_FLOAT_TAIL_INDEX+2] = 0x80;
//   uart2_buf[UART2_FLOAT_TAIL_INDEX+3] = 0x7f;
	
	
  uint8_t none[4]= {1,2,3,4};
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // float local_etheta ;
    // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
    //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    // __HAL_TIM_DISABLE_IT(&htim6, TIM_IT_UPDATE); 
    // local_etheta = etheta;
    // __HAL_TIM_ENABLE_IT(&htim6, TIM_IT_UPDATE); 
    // Anti_Park_Transform(Ud,Uq,local_etheta,&Ualpha,&Ubeta);
    // SVPWM(Ualpha,Ubeta,VBarbus,PWM_cmp,PWM_cmp+1,PWM_cmp+2);
    // __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, PWM_cmp[0]*8000);
    // __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, PWM_cmp[1]*8000);
    // __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, PWM_cmp[2]*8000);
    
    // memcpy(tx_buf,(uint8_t*)&etheta_angle,4);
    // memcpy(tx_buf+4,&glb_section,4);
    // memcpy(tx_buf+8,PWM_cmp,12);
    // memcpy(tx_buf+8,&Ud,4);
    // memcpy(tx_buf+12,&Uq,4);
    // memcpy(tx_buf+16,&Ualpha,4);
    // memcpy(tx_buf+20,&Ubeta,4);
    // memcpy(tx_buf+24,&glb_Ua,4);
    // memcpy(tx_buf+28,&glb_Ub,4);
    // memcpy(tx_buf+32,&glb_Uc,4);
    // memcpy(tx_buf+36,&glb_vx,4);
    // memcpy(tx_buf+40,&glb_vy,4);
    // memcpy(tx_buf+44,&glb_vz,4);
    // memcpy(tx_buf+48,&glb_t0,4);
    // memcpy(tx_buf+52,&glb_t1,4);
    // memcpy(tx_buf+56,&glb_t2,4);
    

		//memcpy(tx_buf+4,PWM_cmp,12);
		//HAL_UART_Transmit_DMA(&huart1,tx_buf,TX_NUM);

    //  HAL_UART_Transmit(&huart1,none,4,100);
    //  HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_4,(uint32_t*)LED_array,25);
   // if(htim->Instance==TIM6)
  // {
  //   etheta += 0.1000f;
	// 	if(etheta>6.28f)
	// 		etheta =0.0f;
  //   else if(etheta<-0.0f)
  //     etheta = 6.28f;

  //   etheta_angle= etheta / 3.1415926 *180;
  // }
    HAL_Delay(1);
		// HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_4,(uint32_t*)LED_array,25);
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
#ifdef USE_FULL_ASSERT
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
