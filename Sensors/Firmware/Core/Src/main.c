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
#include "registers.h"
#include "RFM69W.h"
#include "packet.h"
#include <string.h>


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;

uint8_t  *RF_ModuleName;                                        //Module name cache
uint16_t *RF_FreqBuf;                                           //RF center frequency cache
uint8_t  *RF_RateBuf;                                           //RF rate cache
uint8_t  *RF_PowerBuf;                                          //RF power cache
uint8_t  *RF_FdevBuf;                                           //RF deviation cache
uint8_t  *RF_BandBuf;                                           //RF bandwidth cache
uint8_t  *RF_RSSIBuf;                                           //RF RSSI cache
uint8_t  *RF_ModemBuf;                                          //RF modem cache
uint16_t *RF_LoRaBandWidthBuf;                                  //RF LoRa bandwidth cache

const uint8_t  C_RFM69_ModuleName[]={"69 "};
const uint16_t C_RFM69_FreqBuf[]={315,434,868,915,0};
const uint8_t  C_RFM69_RateBuf[]={12,24,48,96,0};
const uint8_t  C_RFM69_PowerBuf[]={13,10,7,4,0};
const uint8_t  C_RFM69_FdevBuf[]={35,0};
const uint8_t  C_RFM69_BandBuf[]={83,0};
const uint8_t  C_RFM69_RSSIBuf[]={20,50,80,110,0};
const uint8_t  C_RFM69_ModemBuf[]={C_SysMode_FSK,0};

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**********************************************************
**Name:     InitSystemParameter
**Function: Initial system parameter
**Input:    none
**Output:   none
**********************************************************/
void InitSystemParameter(void)
{
  gw_SendDataCount=0;                                      //Send data count
  gw_ReceiveDataCount=0;                                   //Receive data count

  if(RF_ModemBuf[1]==0 && gb_SystemMode==C_SysMode_Modem){gb_SystemMode=C_SysMode_Set;}      //Direct entry set mode when the modem mode only one

  gw_RF_SentInterval=C_RF_SentInterval;                    //RF parameter Send interval time(ms)

  RF_ModemBuf=(uint8_t*)C_RFM69_ModemBuf;
  if(RF_ModemBuf[gb_Modem_Addr]==C_SysMode_FSK)
  {
    RF_ModuleName=(uint8_t*)C_RFM69_ModuleName;
    RF_FreqBuf=(uint16_t*)C_RFM69_FreqBuf;
    RF_RateBuf=(uint8_t*)C_RFM69_RateBuf;
    RF_PowerBuf=(uint8_t*)C_RFM69_PowerBuf;
    RF_FdevBuf=(uint8_t*)C_RFM69_FdevBuf;
    RF_BandBuf=(uint8_t*)C_RFM69_BandBuf;
    RF_RSSIBuf=(uint8_t*)C_RFM69_RSSIBuf;
  }

  if(gb_FirstPowerUpFlag==1)                               //FirstPowerUp
  {
    if((gb_SystemMode>=C_SysMode_FSKTx && gb_SystemMode<=C_SysMode_LoRaTRx) || (gb_SystemMode>=C_SysMode_TestTx && gb_SystemMode<=C_SysMode_Sleep))
    {
      switch(RF_ModemBuf[gb_Modem_Addr])
      {
        case C_SysMode_FSK:
          gb_ModuleWorkMode=C_ModuleWorkMode_FSK;          //Module entry FSK mode
          break;
        case C_SysMode_OOK:
          gb_ModuleWorkMode=C_ModuleWorkMode_OOK;          //Module entry OOK mode
          break;
        case C_SysMode_LoRa:
          gb_ModuleWorkMode=C_ModuleWorkMode_LoRa;         //Module entry LoRa mode
          break;
      }
      //if(gb_SystemMode==C_SysMode_Standby){gb_ModuleWorkMode=C_ModuleWorkMode_Standby;}   //Module entry standby mode
      if(gb_SystemMode==C_SysMode_Sleep){gb_ModuleWorkMode=C_ModuleWorkMode_Sleep;}       //Module entry sleep mode

      gb_ModuleWorkEnableFlag=1;                           //Enable module work in Tx/Rx mode
      gb_ParameterChangeFlag=1;                            //Enalbe entry normal mode
    }
    else
    {
      if(gb_SystemMode==C_SysMode_EntrySet)
      {
        gb_SysMode_Set=C_SysMode_Set_Freq;                 //Modify SysMode set
      }
      gb_ModuleWorkEnableFlag=0;                           //Disable module work in Tx/Rx mode
      gb_ModuleWorkMode=C_ModuleWorkMode_Standby;          //Module entry stanby mode
      gb_ParameterChangeFlag=2;
    }
  }
}

/**********************************************************
**Name:     ModuleSelectModeEntryCheck
**Function: Confirm whether you can enter module select mode
**Input:    none
**Output:   none
**********************************************************/
void ModuleSelectModeEntryCheck(void)
{
  gb_FirstPowerUpFlag=1;                                   //FirstPowerUp

    gb_FreqBuf_Addr=0;
    gb_RateBuf_Addr=1;
    gb_PowerBuf_Addr=0;
    gb_FdevBuf_Addr=0;
    gb_BandBuf_Addr=0;
    gb_Modem_Addr_Backup=gb_Modem_Addr=0;

    gb_SystemMode=C_SysMode_Modem;                       //RF select modem mode

  InitSystemParameter();
}


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
  MX_SPI2_Init();
  MX_TIM1_Init();
  HAL_TIM_Base_Start_IT(&htim1);
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */

  uint8_t TxFlag=0;
  uint8_t RxFlag=0;

   //timer1_init();
   //timer2_init();
   ModuleSelectModeEntryCheck();                            //Confirm whether you can enter module select mode
   SWS_Packet packet;
   DataBuffer buffer;
   SmokeSensorData smokeData;
   packet.packetID = 0;

   gb_SystemMode=C_SysMode_FSKTx;
   packet.idBase = 1;
   packet.idSensor = 2;
   packet.packetID = packet.packetID++;
   packet.packetType = SEND;
   smokeData.GasDetectionTimeout_Minutes = 10;
   smokeData.GasSensor = 500;
   smokeData.SmokeSensor = 1000;
   smokeData.SensorType = 2;
   smokeData.Temperature = 52;
   memcpy(packet.data, &smokeData, 8);

   packet_serializeData(&packet, &buffer);

   while(1)
   {


	 if(gb_ModuleWorkEnableFlag)
	 {
		//gb_SystemMode=C_SysMode_FSKTx;	//Send

		RFM69_Running(gb_SystemMode,gb_ModuleWorkMode,gb_ParameterChangeFlag,&TxFlag,&RxFlag,&gb_RF_RSSI, &buffer);

		gb_SystemMode = C_ModuleWorkMode_Standby;
		//gb_SystemMode=C_SysMode_FSKTx;	//Receive (only if ACK needed)

		//RFM69_Running(gb_SystemMode,gb_ModuleWorkMode,gb_ParameterChangeFlag,&TxFlag,&RxFlag,&gb_RF_RSSI);

		if(TxFlag==1)                                        //Sent successfully
		{
		 TxFlag=0;
		 gw_SendDataCount++;
		 if(gw_SendDataCount>9999){gw_SendDataCount=0;}
		}
		if(RxFlag==1)                                        //Successfully received
		{
		 RxFlag=0;
		 gw_ReceiveDataCount++;
		 if(gw_ReceiveDataCount>9999){gw_ReceiveDataCount=0;}
		}

		if(gb_ParameterChangeFlag==1){gb_ParameterChangeFlag=0;}    //clear parameter flag
		if(gb_ModuleWorkMode!=C_ModuleWorkMode_FSK && gb_ModuleWorkMode!=C_ModuleWorkMode_OOK && gb_ModuleWorkMode!=C_ModuleWorkMode_LoRa)
		{
		 gb_StatusTx=0;
		 gb_StatusRx=0;
		}
		}
		else
		{
		gb_StatusTx=0;
		gb_StatusRx=0;
		}
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  htim1.Init.Prescaler = 1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 36000;
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
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DIO0_Interrupt_Pin */
  GPIO_InitStruct.Pin = DIO0_Interrupt_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DIO0_Interrupt_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CUSTOMNAME_Pin */
  GPIO_InitStruct.Pin = CUSTOMNAME_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CUSTOMNAME_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Sensor0_Pin Sensor1_Pin Sensor2_Pin Sensor3_Pin */
  GPIO_InitStruct.Pin = Sensor0_Pin|Sensor1_Pin|Sensor2_Pin|Sensor3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
