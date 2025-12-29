/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

////////////////////////////////////////////////////////////////////////////////
volatile int Fuzzy_Matrix[3][3];
volatile int NE,NDE,PE,PDE,ZE,ZDE,err_sum,error,d_err,Kp,Ki,Kd,prev_err;
//volatile float
volatile int PWM_Fuzzy,ticks;
volatile int target=50;
volatile int Kp_small,Kp_medium,Kp_large;
volatile int Ki_small,Ki_medium,Ki_large;
volatile int Kd_small,Kd_medium,Kd_large;
volatile int out;
const int Kp_s=1,Kp_m=2,Kp_l=3;
const int Ki_s=1,Ki_m=2,Ki_l=3;
const int Kd_s=1,Kd_m=2,Kd_l=3;
////////////////////////////////////////////////////////////////////////////////

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

////////////////////////////////////////////////////////////////////////////////
void init(){
	int i,j;
	NE=0; PDE=0; NDE=0; PE=0; ZE=0; ZDE=0;
	Kp=0; Ki=0; Kd=0;
	error=0;
	prev_err=0;
	PWM_Fuzzy=0;
	err_sum=0;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++) Fuzzy_Matrix[i][j]=0;
	}
}

int max(int x,int y){
	return x>y?x:y;
}

int min(int x,int y){
	return x>y?y:x;
}

void Fuzzification(){
	NE=0; PDE=0; NDE=0; PE=0;
	if(error<=-20) NE=20;
	else if(error>=20) PE=20;
	else{
		if(error>=0){
			PE=error;
			NDE=20-error;
		}
		else{
			NE=-error;
			ZDE=20+error;
		}
	}
	if(d_err<=-20) NDE=20;
	else if(d_err>=20) PDE=20;
	else{
		if(d_err>=0){
			PDE=d_err;
			ZDE=20-d_err;
		}
		else{
			NDE=-d_err;
			ZDE=20+d_err;
		}
	}
}

void Create_Fuzzy_Matrix(){
	Fuzzy_Matrix[0][0]=min(NE,NDE);
	Fuzzy_Matrix[0][1]=min(NE,ZDE);
	Fuzzy_Matrix[0][2]=min(NE,PDE);
	Fuzzy_Matrix[1][0]=min(ZE,NDE);
	Fuzzy_Matrix[1][1]=min(ZE,ZDE);
	Fuzzy_Matrix[1][2]=min(ZE,PDE);
	Fuzzy_Matrix[2][0]=min(PE,NDE);
	Fuzzy_Matrix[2][1]=min(PE,ZDE);
	Fuzzy_Matrix[2][2]=min(PE,PDE);
}

void Defuzzification(){
	Kp_small=max(Fuzzy_Matrix[0][0],Fuzzy_Matrix[0][1]);
	Kp_large=max(Fuzzy_Matrix[2][1],Fuzzy_Matrix[2][2]);
	Kp_medium=max(Fuzzy_Matrix[0][1],max(Fuzzy_Matrix[1][0],max(Fuzzy_Matrix[1][1],max(Fuzzy_Matrix[1][2],Fuzzy_Matrix[2][1]))));

	Ki_small=max(Fuzzy_Matrix[0][0],Fuzzy_Matrix[0][1]);
	Ki_large=max(Fuzzy_Matrix[2][1],Fuzzy_Matrix[2][2]);
	Ki_medium=max(Fuzzy_Matrix[0][1],max(Fuzzy_Matrix[1][0],max(Fuzzy_Matrix[1][1],max(Fuzzy_Matrix[1][2],Fuzzy_Matrix[2][1]))));

	Kd_large=max(Fuzzy_Matrix[0][0],Fuzzy_Matrix[0][1]);
	Kd_small=max(Fuzzy_Matrix[2][1],Fuzzy_Matrix[2][2]);
	Kd_medium=max(Fuzzy_Matrix[0][1],max(Fuzzy_Matrix[1][0],max(Fuzzy_Matrix[1][1],max(Fuzzy_Matrix[1][2],Fuzzy_Matrix[2][1]))));


	if(Kp_small!=0 || Kp_medium!=0 || Kp_large!=0) Kp=(Kp_s*Kp_small+Kp_m*Kp_medium+Kp_l*Kp_large)/(Kp_small+Kp_medium+Kp_large);
	if(Ki_small!=0 || Ki_medium!=0 || Ki_large!=0) Ki=(Ki_s*Ki_small+Ki_m*Ki_medium+Ki_l*Ki_large)/(Ki_small+Ki_medium+Ki_large);
	if(Kd_small!=0 || Kd_medium!=0 || Kd_large!=0) Kd=(Kd_s*Kd_small+Kd_m*Kd_medium+Kd_l*Kd_large)/(Kd_small+Kd_medium+Kd_large);
}

int Compute_PWM(){
	err_sum+=error;
	out=Kp*error + Ki*err_sum + Kd*d_err;
	if(out>127) out=127;
	else if(out<0) out=0;
	prev_err=error;
	return out;
}

//int PWM_Fuzzy = 200;

void Input_Fuzzy(int input)
{

	//Receiving Data as ticks
	ticks=input;
	//Call the Fuzzy Code Function Here and return the value to PWM_Fuzzy
	error=target-ticks;
	d_err=error-prev_err;
	Fuzzification();
	Create_Fuzzy_Matrix();
	Defuzzification();
	PWM_Fuzzy=Compute_PWM();

}
////////////////////////////////////////////////////////////////////////////////

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  //////////////////////////////////////////////////////////////////////////////

  //init();
  //int input_ = 60;
  //Send data
  //Input_Fuzzy(input_);

  //////////////////////////////////////////////////////////////////////////////

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	////////////////////////////////////////////////////////////////////////////
	//Send PWM;
	////////////////////////////////////////////////////////////////////////////

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OUTPUT_GPIO_Port, OUTPUT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : INPUT_Pin */
  GPIO_InitStruct.Pin = INPUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INPUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OUTPUT_Pin */
  GPIO_InitStruct.Pin = OUTPUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OUTPUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
