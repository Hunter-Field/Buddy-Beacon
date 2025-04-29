#include "Display_drivers.h"

extern SPI_HandleTypeDef hspi2;

// Send Command
void Command(uint8_t cmd)
{
    HAL_GPIO_WritePin(OLED_DC_PORT, OLED_DC_PIN, GPIO_PIN_RESET); // Command mode
    HAL_SPI_Transmit(&hspi2, &cmd, 1, 100);
}

// Send Data
void Data(uint8_t data)
{
    HAL_GPIO_WritePin(OLED_DC_PORT, OLED_DC_PIN, GPIO_PIN_SET); // Data mode
    HAL_SPI_Transmit(&hspi2, &data, 1, 100);
}

// Hardware reset
void OLED_Reset(void)
{
    HAL_GPIO_WritePin(OLED_RES_PORT, OLED_RES_PIN, GPIO_PIN_RESET);
    HAL_Delay(1); // Minimum 200us
    HAL_GPIO_WritePin(OLED_RES_PORT, OLED_RES_PIN, GPIO_PIN_SET);
    HAL_Delay(1); // Minimum 200us
}

// Turn display on
void OLED_DisplayOn(void)
{
    Command(OLED_CMD_SET_SLEEP_MODE_OFF);
}

// Turn display off
void OLED_DisplayOff(void)
{
    Command(OLED_CMD_SET_SLEEP_MODE_ON);
}

// Initialize display
void OLED_Init(void)
{
    OLED_Reset();

    Command(OLED_CMD_SET_SLEEP_MODE_ON); // Display off

    // Clock divider and oscillator frequency
    Command(OLED_CMD_SET_CLOCK_DIV);
    Data(0x91); // Divider = 1, Freq = 0x1

    // Set MUX ratio
    Command(OLED_CMD_SET_MULTIPLEX_RATIO);
    Data(0x3F); // 64 MUX (for 64 rows)

    // Set display offset
    Command(OLED_CMD_SET_DISP_OFFSET);
    Data(0x00); // No offset

    // Function selection
    Command(OLED_CMD_FUNC_SELECT);
    Data(0x01); // Internal VDD regulator

    // Set re-map (memory addressing)
    Command(OLED_CMD_SET_REMAP);
    Data(0x14); // Horizontal increment, column address 0 mapped to SEG0
    Data(0x11); // Enable COM split odd/even

    // Master contrast current control
    Command(OLED_CMD_MASTER_CONTRAST_CURRENT);
    Data(0x0F); // Maximum contrast

    // Set contrast current
    Command(OLED_CMD_SET_CONTRAST_CURRENT);
    Data(0x9F); // Medium contrast

    // Set phase length
    Command(OLED_CMD_SET_PHASE_LENGTH);
    Data(0xE2); // Phase 1 = 15 DCLKs, Phase 2 = 5 DCLKs

    // Set pre-charge voltage
    Command(OLED_CMD_SET_PRECHARGE_VOLTAGE);
    Data(0x1F); // ~0.6*VCC

    // Set VSL
    Command(OLED_CMD_SET_SECOND_PRECHARGE);
    Data(0x08); // Second precharge period

    // Set VCOMH voltage
    Command(OLED_CMD_SET_VCOMH_VOLTAGE);
    Data(0x04); // ~0.8*VCC

    // Set display mode to normal
    Command(OLED_CMD_DISP_MODE_NORMAL);

    // Clear display
//    OLED_Clear();

    // Turn on display
    Command(OLED_CMD_SET_SLEEP_MODE_OFF);
}

// Set every pixel white
void OLED_Clear(void)
{
    OLED_Reset_Column();
    OLED_Reset_Row();
    Command(OLED_CMD_WRITE_RAM);

	for (uint16_t i = 0; i < 8192; i++) {
		Data(0xFF);
	}
}

// Reset the column
void OLED_Reset_Column(void) {
    Command(OLED_CMD_SET_COL_ADDR);
    Data(0x1C);
    Data(0x5B);
}

// Reset the row
void OLED_Reset_Row(void) {
    Command(OLED_CMD_SET_ROW_ADDR);
    Data(0x00);
    Data(0xCF);
}

// Set the display window
void OLED_SetWindow(void)
{
	OLED_Reset_Column();
	OLED_Reset_Row();
}

// Draw the bitmap array onto the OLED
void OLED_DrawBitmap(uint8_t x, uint8_t y, const uint16_t *bitmap)
{
	OLED_Reset_Column();
	OLED_Reset_Row();
	Command(OLED_CMD_WRITE_RAM);

	for (int row = 0; row < OLED_HEIGHT; row++) {
		for (int col = OLED_WIDTH - 1; col >= 0; col--) {
			int index = (row * OLED_WIDTH + col); // Get index for 16-bit pixel in bitmap

			uint16_t color = bitmap[index];

			// unpack RGB565
			uint8_t r5 = (color >> 11) & 0x1F;
			uint8_t g6 = (color >>  5) & 0x3F;
			uint8_t b5 =  color        & 0x1F;

			// expand to 8 bits
			uint8_t r8 = (r5 << 3) | (r5 >> 2);  // 5→8
			uint8_t g8 = (g6 << 2) | (g6 >> 4);  // 6→8
			uint8_t b8 = (b5 << 3) | (b5 >> 2);  // 5→8

			// approximate luma in 0–255
			uint16_t luma = (r8 * 299 + g8 * 587 + b8 * 114 + 500) / 1000;

			// map 0–255 down to 0–63
			uint8_t gray6 = (luma * 63 + 127) / 255;

			// Invert grayscale and apply threshold (pure black or white)
			if (gray6 <= 40) {
				Data(0xFF);  // black
			} else {
				Data(0x00);  // white
			}
		}
	}
}
