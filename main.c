/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "ads1115.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define DBG_UART 1

unsigned char ADSwrite[6];



int16_t reading;
float voltage[4];
//const float voltageConv = 6.114 / 32768.0;



//#define I2C_ID_ADDRESS                                           0x48
//#define I2C_TIMEOUT                                              10

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */


//uint8_t data[3]={0x01,0xf2,0x87};
uint8_t setting[3]={0x01,0x40,0xa3};
uint8_t ser[3];
uint8_t reg_convers[1]={0x00};

uint32_t res, err;

uint8_t channel_0[2];
uint8_t channel_1[2];
uint8_t channel_2[2];
uint8_t channel_3[2];
uint8_t check_setting[2];


uint16_t probeNum;

ADS1115_Config_t configReg;
ADS1115_Handle_t *pADS;




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


uint8_t ADSWrite[6];
int16_t readValue;
int16_t result1;
int16_t result2;
int16_t result3;
float voltValue;
float voltProbes[256];
float ress,resist,themp;

const float voltageConv = 6.114 / 32768.0f;

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
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */


pADS = ADS1115_init(&hi2c2, ADS1115_ADDRESS, configReg);
ADS1115_SetDefault(pADS);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */



	  configReg.pgaConfig = FS6144;
	  configReg.channel = Channel_3;
	  ADS1115_updateConfig(pADS, configReg);


	  HAL_Delay(10);
	  result3 = ADS1115_getData(pADS);
	  ress = raw_to_voltage(result3, pADS);

	  float vreference=3.3;
	  float pull_up_res_Kom=4.7;
	  resist = voltage_to_resistance(ress, vreference, pull_up_res_Kom);

	  //float resist1 = 44.750;
	  themp =resistance_to_temperature(resist);

/*
	  configReg.pgaConfig = FS2048;
	  configReg.channel = Channel_2;
	  ADS1115_updateConfig(pADS, configReg);
	  HAL_Delay(10);
	  result2 = ADS1115_getData(pADS);

	  configReg.pgaConfig = FS4096;
	  configReg.channel = Channel_1;
	  ADS1115_updateConfig(pADS, configReg);
	  HAL_Delay(10);
	  result1 = ADS1115_getData(pADS);
*/


	  HAL_GPIO_TogglePin(user_led7_GPIO_Port, user_led7_Pin);











	  /* read_date_ADS1115(
	 			 W_NE, AIN3_AIN_GND,
	 			 FS4096, CCM,

	 			 SPS475, TCWH,
	 			 AL,NLC,AAFC,

	 			 address_ADS1115,100, &hi2c2,
	 		CONV_REG

	 			 );

	 	  setting_ADS1115(W_NE, AIN3_AIN_GND,
	 			 FS4096, CCM,
	 			 SPS475, TCWH,
	 			 AL,NLC,AAFC
	 			 );*/





/*


  	 HAL_GPIO_TogglePin(user_led7_GPIO_Port, user_led7_Pin);
	 HAL_Delay(30);
	 HAL_GPIO_TogglePin(user_led7_GPIO_Port, user_led7_Pin);

	 //Настройка ads1115 и переключение каналов
	 for( int i = 0; i < 4; i++){
		 setting[0] = 0x01;
		 			switch(i) {
		 				case(0): setting[1] = 0x40;
		 				break;
		 				case(1): setting[1] = 0x50;
		 				break;
		 				case(2): setting[1] = 0x60;
		 				break;
		 				case(3): setting[1] = 0x70;
		 				break;
		 			}
		 	setting[3] = 0xa3;


	 //запись в регистр конфигурации настроек ADS115
  	  HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS<<1, setting, 3, 100);

  	  //чтение конфигурационного регистра для проверки настроек
  	  HAL_I2C_Master_Receive(&hi2c2, ADS1115_ADDRESS<<1, check_setting, 2, 100);

  	  res=0;
  	  if (setting[1]==check_setting[0]&&setting[2]==check_setting[1]){
  		 res=100;
  		HAL_GPIO_TogglePin(user_led5_GPIO_Port, user_led5_Pin);
  	  };

  	  // переключились на регистр результата
  	 // HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS<<1, reg_convers, 1, 100);
  	  HAL_Delay(10);
  	  //старт преобразования

  	  //чтение данных в переменную в зависимости от i
  	 if (i==0){
  		 HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS<<1, setting, 3, 100);
  		 HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS<<1, reg_convers, 1, 100);
  		 HAL_Delay(1);
  		 HAL_I2C_Master_Receive(&hi2c2, ADS1115_ADDRESS<<1, channel_0, 2, 100);
  	  }
  	 HAL_Delay(1);
  	 if (i==1){
  		 HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS<<1, setting, 3, 100);
  		 HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS<<1, reg_convers, 1, 100);
  		 HAL_Delay(1);
  	  	  HAL_I2C_Master_Receive(&hi2c2, ADS1115_ADDRESS<<1, channel_1, 2, 100);
  	 }

  	 if (i==2){
  		 HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS<<1, setting, 3, 100);
  		 HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS<<1, reg_convers, 1, 100);
  		 HAL_Delay(1);
  	  	  HAL_I2C_Master_Receive(&hi2c2, ADS1115_ADDRESS<<1, channel_2, 2, 100);
  	 }
  	 if (i==3){
  		 HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS<<1, setting, 3, 100);
  		 HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS<<1, reg_convers, 1, 100);
  		 HAL_Delay(1);
  	  	  HAL_I2C_Master_Receive(&hi2c2, ADS1115_ADDRESS<<1, channel_3, 2, 100);
  	 }


	 }

  	 //res = HAL_I2C_GetState(&hi2c2);
  	  //err = HAL_I2C_GetError(&hi2c2);
  	  //HAL_Delay(10);
  	 //HAL_I2C_Master_Transmit(&hi2c2, add, data, 1, 100);
  	  //HAL_Delay(10);
*/
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 10;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
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

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 1800-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, user_led1_Pin|user_led5_Pin|user_led6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(user_led3_GPIO_Port, user_led3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(user_led4_GPIO_Port, user_led4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(user_led_GPIO_Port, user_led_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(user_led2_GPIO_Port, user_led2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(user_led7_GPIO_Port, user_led7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : user_led1_Pin user_led6_Pin */
  GPIO_InitStruct.Pin = user_led1_Pin|user_led6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : user_led3_Pin */
  GPIO_InitStruct.Pin = user_led3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(user_led3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : user_led4_Pin */
  GPIO_InitStruct.Pin = user_led4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(user_led4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : user_led_Pin user_led5_Pin */
  GPIO_InitStruct.Pin = user_led_Pin|user_led5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : user_led2_Pin */
  GPIO_InitStruct.Pin = user_led2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(user_led2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : user_led7_Pin */
  GPIO_InitStruct.Pin = user_led7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(user_led7_GPIO_Port, &GPIO_InitStruct);

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
