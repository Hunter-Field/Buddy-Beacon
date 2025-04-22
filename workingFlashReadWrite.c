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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "packets.h"
#include <time.h>
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
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */
#define FLASH_BASE       0x08000000U
#define FLASH_PAGE_SIZE  0x800U      // 2 KB
#define FLASH_PAGES      256U        // pages 0–255

#define MAX_SERIALIZED_SIZE 240
static uint8_t rxBuffer[MAX_SERIALIZED_SIZE];
uint8_t packet_buffer[MAX_SERIALIZED_SIZE];
uint8_t startingIndexOfBufferForPacket = 0;
static volatile bool  rxReady = false;
static volatile uint16_t rxLen   = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void LoRa_Send_AT_Command(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void OLED_SendString(char *myString) {
	/*
	 * This function takes in a string (double quotes only) and
	 * prints it on a 16X2 SOC1602A OLED display. This function accounts for
	 * strings longer than 16 characters by printing the overflow on the second
	 * line and then the rest on a separate first and second line continually.
	 */
	// Initialization commands
	uint8_t init_cmds[] = { 0x38, 0x08, 0x01, 0x06, 0x02, 0x0C };
	for (uint8_t i = 0; i < sizeof(init_cmds); i++) {
		HAL_SPI_Transmit(&hspi1, &init_cmds[i], 1, 100);
	}

	// Convert 8-bit characters to 16-bit and add 0x200 to print to OLED
	uint16_t size = strlen(myString);
	uint16_t charList[size];
	for (uint16_t i = 0; i < size; i++) {
		charList[i] = (uint16_t) myString[i] + 0x200;
	}

	// Print string in chunks of 32 characters
	uint16_t start = 0;
	while (start < size) {
		// Clear the display
		SPI1->DR = 0x01;
		HAL_Delay(10);

		// Move cursor to the first line
		SPI1->DR = 0x02;
		HAL_Delay(10);

		// Move the cursor forward after each character
		// ** Hunter thinks this isn't necessary but removing it breaks everything
		// ** DO NOT REMOVE IT WILL BREAK AFTER MICRO IS UNPLUGGED
		SPI1->DR = 0x06;
		HAL_Delay(10);

		// Print the first 16 characters of current chunk
		for (uint16_t i = start; i < start + 16 && i < size; i++) {
			SPI1->DR = charList[i];
			HAL_Delay(10);
		}

		// Move cursor to the second line
		SPI1->DR = 0xC0;
		HAL_Delay(10);

		// Print the next 16 characters of the current chunk
		for (uint16_t i = start + 16; i < start + 32 && i < size; i++) {
			SPI1->DR = charList[i];
			HAL_Delay(10);
		}

		HAL_Delay(500);
		start += 32;
	}

	HAL_Delay(2000);
}

size_t serializeBeaconPacket(const BeaconPacket *packet, uint8_t *buffer)
{
    if (!packet || !buffer) return 0;
    size_t offset = 0;
    memcpy(buffer + offset, &packet->packetType, sizeof(packet->packetType));
    offset += sizeof(packet->packetType);
    memcpy(buffer + offset, &packet->ttl, sizeof(packet->ttl));
    offset += sizeof(packet->ttl);
    memcpy(buffer + offset, packet->tBeaconPassThrough, 12);
    offset += 12;
    memcpy(buffer + offset, packet->tBeaconID, 12);
    offset += 12;
    memcpy(buffer + offset, packet->rBeaconID, 12);
    offset += 12;

    switch (packet->packetType) {
        case 1:
            memcpy(buffer + offset, packet->data.initBeacon.tBeaconName, 16);
            offset += 16;
            break;
        case 2:
            memcpy(buffer + offset, packet->data.replyNearby.tBeaconName, 16);
            offset += 16;
            break;
        case 4:
            memcpy(buffer + offset, &packet->data.replyCoords.tLatitude, sizeof(float));
            offset += sizeof(float);
            memcpy(buffer + offset, &packet->data.replyCoords.tLongitude, sizeof(float));
            offset += sizeof(float);
            break;
        case 5:
            memcpy(buffer + offset, &packet->data.sendMessage.messageLength, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            memcpy(buffer + offset, packet->data.sendMessage.message, packet->data.sendMessage.messageLength);
            offset += packet->data.sendMessage.messageLength;
            memcpy(buffer + offset, &packet->data.sendMessage.randNum, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            break;
        case 6:
            memcpy(buffer + offset, &packet->data.ackMessage.randNum, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            break;
        case 7:
        {
            memcpy(buffer + offset, &packet->data.beaconList.totalNum, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            memcpy(buffer + offset, &packet->data.beaconList.startingNum, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            memcpy(buffer + offset, &packet->data.beaconList.endingNum, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            uint8_t numBeingSent = packet->data.beaconList.endingNum - packet->data.beaconList.startingNum + 1;
            for (int i = 0; i < numBeingSent; i++) {
                memcpy(buffer + offset, packet->data.beaconList.beacons[i].beaconID, 12);
                offset += 12;
                memcpy(buffer + offset, packet->data.beaconList.beacons[i].beaconName, 16);
                offset += 16;
                memcpy(buffer + offset, &packet->data.beaconList.beacons[i].inRange, sizeof(bool));
                offset += sizeof(bool);
                memcpy(buffer + offset, packet->data.beaconList.beacons[i].accessThroughBeaconID, 12);
                offset += 12;
            }
            break;
        }
        default:
            return 0;
    }
    return offset;
}

void deserializeBeaconPacket(const uint8_t *buffer, uint16_t startIndex, BeaconPacket *packet)
{
    size_t offset = startIndex;
    memcpy(&packet->packetType, buffer + offset, sizeof(packet->packetType));
    offset += sizeof(packet->packetType);
    memcpy(&packet->ttl, buffer + offset, sizeof(packet->ttl));
    offset += sizeof(packet->ttl);
    memcpy(packet->tBeaconPassThrough, buffer + offset, 12);
    offset += 12;
    memcpy(packet->tBeaconID, buffer + offset, 12);
    offset += 12;
    memcpy(packet->rBeaconID, buffer + offset, 12);
    offset += 12;

    switch (packet->packetType) {
        case 1:
            memcpy(packet->data.initBeacon.tBeaconName, buffer + offset, 16);
            break;
        case 2:
            memcpy(packet->data.replyNearby.tBeaconName, buffer + offset, 16);
            break;
        case 4:
            memcpy(&packet->data.replyCoords.tLatitude, buffer + offset, sizeof(float));
            offset += sizeof(float);
            memcpy(&packet->data.replyCoords.tLongitude, buffer + offset, sizeof(float));
            break;
        case 5:
            memcpy(&packet->data.sendMessage.messageLength, buffer + offset, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            memcpy(packet->data.sendMessage.message, buffer + offset, packet->data.sendMessage.messageLength);
            offset += packet->data.sendMessage.messageLength;
            memcpy(&packet->data.sendMessage.randNum, buffer + offset, sizeof(uint16_t));
            break;
        case 6:
            memcpy(&packet->data.ackMessage.randNum, buffer + offset, sizeof(uint16_t));
            break;
        case 7:
        {
            memcpy(&packet->data.beaconList.totalNum, buffer + offset, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            memcpy(&packet->data.beaconList.startingNum, buffer + offset, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            memcpy(&packet->data.beaconList.endingNum, buffer + offset, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            uint8_t numBeingSent = packet->data.beaconList.endingNum - packet->data.beaconList.startingNum + 1;
            for (int i = 0; i < numBeingSent; i++) {
                memcpy(packet->data.beaconList.beacons[i].beaconID, buffer + offset, 12);
                offset += 12;
                memcpy(packet->data.beaconList.beacons[i].beaconName, buffer + offset, 16);
                offset += 16;
                memcpy(&packet->data.beaconList.beacons[i].inRange, buffer + offset, sizeof(bool));
                offset += sizeof(bool);
                memcpy(packet->data.beaconList.beacons[i].accessThroughBeaconID, buffer + offset, 12);
                offset += 12;
            }
            break;
        }
        default:
            return;
    }
}
void LoRa_init(void){
	//set address (100)
	uint8_t address[16] = "AT+ADDRESS=110\r\n";
	uint8_t ntwkID[16] = "AT+NETWORKID=3\r\n";
	uint8_t band[19] = "AT+BAND=915000000\r\n";
	uint8_t param[23] = "AT+PARAMETER=12,7,1,4\r\n";
	uint8_t compare[] = "+OK\r\n";
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, packet_buffer, sizeof(packet_buffer));

	HAL_UART_Transmit(&huart2, address, sizeof(address), 100);
	while(!rxReady);
	rxReady = 0;
	OLED_SendString("Part 1 done");
//	while(strncmp(buffer, compare, 5));
//	OLED_SendString((char *) buffer);
//	memset(buffer, 0, sizeof(buffer));
	HAL_Delay(500);
	//set networkID (3) (needs to be same for both)
	HAL_UART_Transmit(&huart2, ntwkID, sizeof(ntwkID), 100);
	while(!rxReady);
	rxReady = 0;
	HAL_Delay(500);
	OLED_SendString("Part 2 done");

//	HAL_Delay(200);
//	OLED_SendString(buffer);
	//set band (9.15*10^9 Hz, must be same for both (this is default))
	HAL_UART_Transmit(&huart2, band, sizeof(band), 100);
	while(!rxReady);
	rxReady = 0;
	HAL_Delay(500);
	OLED_SendString("Part 3 done");

//	OLED_SendString(buffer);
	//set spreading factor, bandwidth, coding rate, and programmed preamble
	HAL_UART_Transmit(&huart2, param, sizeof(param), 100);
	while(!rxReady);
	rxReady = 0;
	HAL_Delay(500);
	OLED_SendString("All Sequences Set.");
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	rxLen   = Size;
	rxReady = true;
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, packet_buffer, sizeof(packet_buffer));
}
HAL_StatusTypeDef save_data(uint32_t Address, uint32_t data)
{

    FLASH_EraseInitTypeDef EraseInitStruct = {0};
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks = FLASH_BANK_1;
    uint32_t offset = Address - FLASH_BASE;
    uint32_t page = offset / FLASH_PAGE_SIZE;
    EraseInitStruct.Page  = page;
    EraseInitStruct.NbPages     = 1;

    HAL_FLASH_Unlock();

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP   |
                          FLASH_FLAG_OPERR|
                          FLASH_FLAG_WRPERR|
                          FLASH_FLAG_PGAERR|
                          FLASH_FLAG_PGSERR);
    uint32_t PageError;
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK) {
        HAL_FLASH_Lock();
        return HAL_ERROR;
    }

    HAL_StatusTypeDef status = HAL_FLASH_Program(
        FLASH_TYPEPROGRAM_DOUBLEWORD,
        Address,
        (uint64_t)data
    );
    HAL_FLASH_Lock();
    return status;
}

uint32_t read_data(uint32_t Address){

	__IO uint32_t read_data = *(__IO uint32_t *)Address;
	return (uint32_t)read_data;
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
  memset(packet_buffer, 0, sizeof(packet_buffer));

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();

  /* USER CODE BEGIN 2 */
	HAL_Delay(1000);
	//clear display
	uint8_t init_cmds[] = { 0x38, 0x08, 0x01, 0x06, 0x02, 0x0C };
	for (uint8_t i = 0; i < sizeof(init_cmds); i++) {
		HAL_SPI_Transmit(&hspi1, &init_cmds[i], 1, 100);
		HAL_Delay(100);
	}
	OLED_SendString("Resetting screen");
	//LoRa_init();
	HAL_Delay(500);

	uint64_t data=92384;
	save_data(0x0807F800,data);
	uint64_t data1 = 1111;
	//memcpy(&data1, (void*)0x0803F000, sizeof(data1));
	data1 = read_data(0x0807F800);
	if(data1 == data){
		OLED_SendString("Data 1 matches");
	}else{
		OLED_SendString("FAIL");
	}







  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_10BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

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
