#include "main.h"
#include "packets.h"       // Beacon packet definitions and serialization routines
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

SPI_HandleTypeDef hspi1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

#define MAX_MESSAGE_LEN 210
#define MAX_BEACONS 10
#define MAX_SERIALIZED_SIZE 240

// Beacon flash storage variables
BeaconInfo beaconsArray[MAX_BEACONS];
uint32_t startBeaconArrayFlashAddress = 0x080E0000;

// UART receive buffers for LoRa packets (for example purposes)
uint8_t packet_buffer[MAX_SERIALIZED_SIZE] = {0};
uint8_t init_buffer[5] = {0};

// Global flag for initialization mode
uint8_t initMode = 1;

/* Function Prototypes -------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);

void OLED_SendString(char *myString);
void LoRa_init(void);

/* Beacon packet serialize/deserialize function prototypes */
size_t serializeBeaconPacket(const BeaconPacket *packet, uint8_t *buffer);
void deserializeBeaconPacket(const uint8_t *buffer, BeaconPacket *packet);
void printSerializedData(const uint8_t *buffer, size_t size);
int validate_packet(const char *str, size_t len);

/* Beacon flash management and packet handling function prototypes */
void initializeBeaconArray(void);
void storeBeaconsInFlash(void);
void addBeaconToBeaconArray(BeaconInfo beacon);
void handlePacket7(BeaconPacket packet);
void handlePacket1(BeaconPacket packet);
void handlePacket2(BeaconPacket packet);

/* USER CODE BEGIN 0 */
/* Place any additional user code before main() here */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  // Initialize the beacon array in RAM
  initializeBeaconArray();
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
  // Clear the OLED display (using the initialization commands)
  uint8_t init_cmds[] = { 0x38, 0x08, 0x01, 0x06, 0x02, 0x0C };
  for (uint8_t i = 0; i < sizeof(init_cmds); i++) {
      HAL_SPI_Transmit(&hspi1, &init_cmds[i], 1, 100);
      HAL_Delay(100);
  }
  // Start LoRa initialization (which also sets up DMA receive for packet_buffer)
  LoRa_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
    /* USER CODE BEGIN WHILE */
    // Example of sending a packet over LoRa
    BeaconPacket packetToSend;
    memset(&packetToSend, 0, sizeof(packetToSend));
    packetToSend.packetType = 1; // Example: Packet01 (Init Beacon)
    packetToSend.ttl = 0;
    snprintf(packetToSend.tBeaconPassThrough, sizeof(packetToSend.tBeaconPassThrough), "");
    snprintf(packetToSend.tBeaconID, sizeof(packetToSend.tBeaconID), "T12345678901");
    snprintf(packetToSend.data.initBeacon.tBeaconName, sizeof(packetToSend.data.initBeacon.tBeaconName), "Beacon A");

    uint8_t serializedBuffer[MAX_SERIALIZED_SIZE] = {0};
    size_t serializedSize = serializeBeaconPacket(&packetToSend, serializedBuffer);
    
    // Transmit serialized data over UART (LoRa module) 
    HAL_UART_Transmit(&huart2, serializedBuffer, serializedSize, 100);

    // Display on OLED that a packet was sent
    OLED_SendString("Packet sent!");

    // Example: wait for a packet reception using DMA/IDLE callback.
    HAL_Delay(2000);
    
    // Process received packet if any (this example assumes packet_buffer holds a complete packet)
    if (packet_buffer[0] != 0) {
        BeaconPacket receivedPacket;
        deserializeBeaconPacket(packet_buffer, &receivedPacket);
        // Handle the packet based on its type
        switch(receivedPacket.packetType) {
            case 1:
                handlePacket1(receivedPacket);
                break;
            case 2:
                handlePacket2(receivedPacket);
                break;
            case 7:
                handlePacket7(receivedPacket);
                break;
            default:
                OLED_SendString("Unknown Packet");
                break;
        }
        OLED_SendString(receivedPacket.tBeaconID);
        memset(packet_buffer, 0, sizeof(packet_buffer));
    } else {
        OLED_SendString("No packet received");
    }
    /* USER CODE END WHILE */
    
    /* USER CODE BEGIN 3 */
    HAL_Delay(1000);
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

  /* Configure the main internal regulator output voltage */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
      Error_Handler();
  }

  /* Initializes the RCC Oscillators */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
      Error_Handler();
  }
  
  /* Initializes the CPU, AHB and APB buses clocks */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{
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
}

/**
  * @brief DMA Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA_Init(void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init for USART2 RX */
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
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /* Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);

  /* Configure GPIO pin : PC14 (for example, status LED) */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/* HAL UART Idle/DMA Reception Callback */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    // Restart reception on idle event
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, packet_buffer, sizeof(packet_buffer));
}

/* OLED display routine (prints in chunks if string is long) */
void OLED_SendString(char *myString)
{
    uint8_t init_cmds[] = { 0x38, 0x08, 0x01, 0x06, 0x02, 0x0C };
    for (uint8_t i = 0; i < sizeof(init_cmds); i++) {
        HAL_SPI_Transmit(&hspi1, &init_cmds[i], 1, 100);
    }
    uint16_t size = strlen(myString);
    uint16_t charList[size];
    for (uint16_t i = 0; i < size; i++) {
        charList[i] = (uint16_t) myString[i] + 0x200;
    }
    uint16_t start = 0;
    while (start < size) {
        SPI1->DR = 0x01; // Clear display
        HAL_Delay(10);
        SPI1->DR = 0x02; // Move cursor to first line
        HAL_Delay(10);
        SPI1->DR = 0x06; // Set entry mode (required)
        HAL_Delay(10);
        for (uint16_t i = start; i < start + 16 && i < size; i++) {
            SPI1->DR = charList[i];
            HAL_Delay(10);
        }
        SPI1->DR = 0xC0; // Move cursor to second line
        HAL_Delay(10);
        for (uint16_t i = start + 16; i < start + 32 && i < size; i++) {
            SPI1->DR = charList[i];
            HAL_Delay(10);
        }
        HAL_Delay(500);
        start += 32;
    }
    HAL_Delay(2000);
}

/* LoRa initialization routine: sets various parameters and starts DMA reception */
void LoRa_init(void)
{
    uint8_t address[16] = "AT+ADDRESS=110\r\n";
    uint8_t ntwkID[16] = "AT+NETWORKID=3\r\n";
    uint8_t band[19] = "AT+BAND=915000000\r\n";
    uint8_t param[23] = "AT+PARAMETER=12,7,1,4\r\n";

    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, packet_buffer, sizeof(packet_buffer));

    HAL_UART_Transmit(&huart2, address, sizeof(address), 100);
    HAL_Delay(500);
    HAL_UART_Transmit(&huart2, ntwkID, sizeof(ntwkID), 100);
    HAL_Delay(500);
    HAL_UART_Transmit(&huart2, band, sizeof(band), 100);
    HAL_Delay(500);
    HAL_UART_Transmit(&huart2, param, sizeof(param), 100);
    HAL_Delay(500);
    initMode = 0;
    OLED_SendString("All Sequences Set.");
}

/*--------------------- Beacon Packet Serialization Routines ---------------------*/
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

void deserializeBeaconPacket(const uint8_t *buffer, BeaconPacket *packet)
{
    if (!buffer || !packet) return;
    size_t offset = 0;
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

void printSerializedData(const uint8_t *buffer, size_t size)
{
    printf("Serialized Packet Data (%zu bytes):\n", size);
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", buffer[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");
}

int validate_packet(const char *str, size_t len)
{
    if (len == 0 || len > MAX_MESSAGE_LEN) {
        return 0;
    }
    uint8_t firstByte = (uint8_t)str[0];
    return (firstByte <= 7) ? 1 : 0;
}

/*------------------ Beacon Flash Management and Packet Handlers ------------------*/
void initializeBeaconArray(void)
{
    for (int i = 0; i < MAX_BEACONS; i++) {
        beaconsArray[i].beaconID[0] = '\0';
        beaconsArray[i].beaconName[0] = '\0';
        beaconsArray[i].inRange = false;
        beaconsArray[i].accessThroughBeaconID[0] = '\0';
    }
}

void storeBeaconsInFlash(void)
{
    HAL_FLASH_Unlock();
    // Erase sector 11 (adjust the sector and voltage range according to your device)
    FLASH_Erase_Sector(FLASH_SECTOR_11, VOLTAGE_RANGE_3);
    HAL_FLASH_Lock();
    for (int i = 0; i < MAX_BEACONS; i++) {
        // Program the beacon info word-by-word into flash.
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startBeaconArrayFlashAddress + (i * sizeof(BeaconInfo)), (uint32_t)&beaconsArray[i]);
    }
}

void addBeaconToBeaconArray(BeaconInfo beacon)
{
    for (int j = 0; j < MAX_BEACONS; j++ )
    {
        // Check if beacon already exists; if so, update it.
        if (strcmp(beaconsArray[j].beaconID, beacon.beaconID) == 0) {
            strcpy(beaconsArray[j].beaconName, beacon.beaconName);
            beaconsArray[j].inRange = beacon.inRange;
            strcpy(beaconsArray[j].accessThroughBeaconID, beacon.accessThroughBeaconID);
            break;
        }
        // Otherwise, add it if the entry is empty (first character is null) or marked as not in range.
        else if ((beaconsArray[j].beaconID[0] == '\0') || (beaconsArray[j].inRange == false)) {
            strcpy(beaconsArray[j].beaconID, beacon.beaconID);
            strcpy(beaconsArray[j].beaconName, beacon.beaconName);
            beaconsArray[j].inRange = beacon.inRange;
            strcpy(beaconsArray[j].accessThroughBeaconID, beacon.accessThroughBeaconID);
            break;
        }
    }
    // Save updated beacon array to flash
    storeBeaconsInFlash();
}

void handlePacket7(BeaconPacket packet)
{
    uint8_t numBeacons = packet.data.beaconList.endingNum - packet.data.beaconList.startingNum + 1;
    for (int i = 0; i < numBeacons; i++) {
        BeaconInfo beacon;
        memcpy(beacon.beaconID, packet.data.beaconList.beacons[i].beaconID, 12);
        memcpy(beacon.beaconName, packet.data.beaconList.beacons[i].beaconName, 16);
        beacon.inRange = packet.data.beaconList.beacons[i].inRange;
        // Use the transmitter beacon ID as the access-through ID
        memcpy(beacon.accessThroughBeaconID, packet.tBeaconID, 12);
        addBeaconToBeaconArray(beacon);
    }
}

void handlePacket1(BeaconPacket packet)
{
    BeaconInfo newBeacon;
    memcpy(newBeacon.beaconID, packet.tBeaconID, 12);
    memcpy(newBeacon.beaconName, packet.data.initBeacon.tBeaconName, 16);
    newBeacon.inRange = true;
    newBeacon.accessThroughBeaconID[0] = '\0';
    addBeaconToBeaconArray(newBeacon);
}

void handlePacket2(BeaconPacket packet)
{
    BeaconInfo newBeacon;
    memcpy(newBeacon.beaconID, packet.tBeaconID, 12);
    memcpy(newBeacon.beaconName, packet.data.replyNearby.tBeaconName, 16);
    newBeacon.inRange = true;
    newBeacon.accessThroughBeaconID[0] = '\0';
    addBeaconToBeaconArray(newBeacon);
}

/* Error Handler */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add implementation to report the file name and line number */
}
#endif /* USE_FULL_ASSERT */
