#ifndef OLED_H
#define OLED_H

#include "stm32l4xx_hal.h"
#include <string.h>
#include <stdlib.h>

// Display dimensions
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// Pin definitions
#define OLED_RES_PIN GPIO_PIN_11
#define OLED_RES_PORT GPIOB
#define OLED_DC_PIN GPIO_PIN_14
#define OLED_DC_PORT GPIOB

// Command definitions
#define OLED_CMD_SET_COL_ADDR 0x15
#define OLED_CMD_SET_ROW_ADDR 0x75
#define OLED_CMD_WRITE_RAM 0x5C
#define OLED_CMD_READ_RAM 0x5D
#define OLED_CMD_SET_REMAP 0xA0
#define OLED_CMD_SET_DISP_START_LINE 0xA1
#define OLED_CMD_SET_DISP_OFFSET 0xA2
#define OLED_CMD_DISP_MODE_OFF 0xA4
#define OLED_CMD_DISP_MODE_ON 0xA5
#define OLED_CMD_DISP_MODE_NORMAL 0xA6
#define OLED_CMD_DISP_MODE_INVERSE 0xA7
#define OLED_CMD_ENABLE_PARTIAL_DISP 0xA8
#define OLED_CMD_EXIT_PARTIAL_DISP 0xA9
#define OLED_CMD_FUNC_SELECT 0xAB
#define OLED_CMD_SET_SLEEP_MODE_ON 0xAE
#define OLED_CMD_SET_SLEEP_MODE_OFF 0xAF
#define OLED_CMD_SET_PHASE_LENGTH 0xB1
#define OLED_CMD_SET_CLOCK_DIV 0xB3
#define OLED_CMD_SET_GPIO 0xB5
#define OLED_CMD_SET_SECOND_PRECHARGE 0xB6
#define OLED_CMD_SET_GRAYSCALE_TABLE 0xB8
#define OLED_CMD_SELECT_LINEAR_GS_TABLE 0xB9
#define OLED_CMD_SET_PRECHARGE_VOLTAGE 0xBB
#define OLED_CMD_SET_VCOMH_VOLTAGE 0xBE
#define OLED_CMD_SET_CONTRAST_CURRENT 0xC1
#define OLED_CMD_MASTER_CONTRAST_CURRENT 0xC7
#define OLED_CMD_SET_MULTIPLEX_RATIO 0xCA
#define OLED_CMD_SET_COMMAND_LOCK 0xFD

// Display buffer
extern uint8_t OLED_buffer[OLED_WIDTH * OLED_HEIGHT / 2]; // 4-bit grayscale

// Function prototypes
void Command(uint8_t cmd);
void Data(uint8_t data);

void OLED_Init(void);
void OLED_Reset(void);
void OLED_DisplayOn(void);
void OLED_DisplayOff(void);
void OLED_Clear(void);
void OLED_Reset_Column(void);
void OLED_Reset_Row(void);
void OLED_SetWindow(void);
void OLED_DrawBitmap(uint8_t x, uint8_t y, const uint16_t *bitmap);

#endif // OLED_H
