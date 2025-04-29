#include "keypad.h"

extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart2;

// Pins 67-64
GPIO_TypeDef* rowPorts[4] = {GPIOA, GPIOC, GPIOC, GPIOC};
uint16_t rowPins[4] = {GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_8, GPIO_PIN_7};

// Pins 63-61
GPIO_TypeDef* colPorts[3] = {GPIOC, GPIOD, GPIOD};
uint16_t colPins[3] = {GPIO_PIN_6, GPIO_PIN_15, GPIO_PIN_14};

char keyMap[4][3] = {
	  {'1','2','3'},
	  {'4','5','6'},
	  {'7','8','9'},
	  {'*','0','#'}
};

char keyBuffer[MESSAGE_LENGTH];
uint8_t bufferIndex = 0;
char message_buffer[MESSAGE_LENGTH];

char Keypad_Scan(void) {
    char key = '\0';

    // Scan each row
    for (uint8_t row = 0; row < 4; row++) {
        HAL_GPIO_WritePin(rowPorts[row], rowPins[row], GPIO_PIN_SET);
        HAL_Delay(1);

        for (uint8_t col = 0; col < 3; col++) {
            if (HAL_GPIO_ReadPin(colPorts[col], colPins[col])) {
                key = keyMap[row][col];
                break;
            }
        }

        HAL_GPIO_WritePin(rowPorts[row], rowPins[row], GPIO_PIN_RESET);

        if (key != '\0') {
            break;
        }
    }

    return key;
}

void Keypad_Update() {
	if(bufferIndex == 4){
		snprintf(message_buffer, sizeof(message_buffer), "AT+SEND=110,4,%s\r\n", (char*)keyBuffer);
		while(1){
			HAL_UART_Transmit(&huart2, message_buffer, 20, 100);
			HAL_Delay(10000);
		}
	}

	char key = '\0';

	if(Keypad_Scan() == '1'){
		key = 'a';
		keyBuffer[bufferIndex] = key;
		Keypad_Refresh_Screen(keyBuffer);
		HAL_Delay(100);

		if(Keypad_Scan() == '1'){
			key = 'b';
			keyBuffer[bufferIndex] = key;
			Keypad_Refresh_Screen(keyBuffer);
			HAL_Delay(100);

			if(Keypad_Scan() == '1'){
				key = 'c';
				keyBuffer[bufferIndex] = key;
				Keypad_Refresh_Screen(keyBuffer);
				HAL_Delay(100);
			}
		}
	}
	else if(Keypad_Scan() == '2'){
		key = 'd';
		keyBuffer[bufferIndex] = key;
		Keypad_Refresh_Screen(keyBuffer);
		HAL_Delay(100);

		if(Keypad_Scan() == '2'){
			key = 'e';
			keyBuffer[bufferIndex] = key;
			Keypad_Refresh_Screen(keyBuffer);
			HAL_Delay(100);

			if(Keypad_Scan() == '2'){
				key = 'f';
				keyBuffer[bufferIndex] = key;
				Keypad_Refresh_Screen(keyBuffer);
				HAL_Delay(100);
			}
		}

		HAL_Delay(200);
	}
	else if(Keypad_Scan() == '3'){
		key = 'g';
		keyBuffer[bufferIndex] = key;
		Keypad_Refresh_Screen(keyBuffer);
		HAL_Delay(100);

		if(Keypad_Scan() == '3'){
			key = 'h';
			keyBuffer[bufferIndex] = key;
			Keypad_Refresh_Screen(keyBuffer);
			HAL_Delay(100);

			if(Keypad_Scan() == '3'){
				key = 'i';
				keyBuffer[bufferIndex] = key;
				Keypad_Refresh_Screen(keyBuffer);
				HAL_Delay(100);
			}
		}

		HAL_Delay(200);
	}
	else if(Keypad_Scan() == '4'){
		key = 'j';
		keyBuffer[bufferIndex] = key;
		Keypad_Refresh_Screen(keyBuffer);
		HAL_Delay(100);

		if(Keypad_Scan() == '4'){
			key = 'k';
			keyBuffer[bufferIndex] = key;
			Keypad_Refresh_Screen(keyBuffer);
			HAL_Delay(100);

			if(Keypad_Scan() == '4'){
				key = 'l';
				keyBuffer[bufferIndex] = key;
				Keypad_Refresh_Screen(keyBuffer);
				HAL_Delay(100);
			}
		}

		HAL_Delay(200);
	}
	else if(Keypad_Scan() == '5'){
		key = 'm';
		keyBuffer[bufferIndex] = key;
		Keypad_Refresh_Screen(keyBuffer);
		HAL_Delay(100);

		if(Keypad_Scan() == '5'){
			key = 'n';
			keyBuffer[bufferIndex] = key;
			Keypad_Refresh_Screen(keyBuffer);
			HAL_Delay(100);

			if(Keypad_Scan() == '5'){
				key = 'o';
				keyBuffer[bufferIndex] = key;
				Keypad_Refresh_Screen(keyBuffer);
				HAL_Delay(100);
			}
		}

		HAL_Delay(200);
	}
	else if(Keypad_Scan() == '6'){
		key = 'p';
		keyBuffer[bufferIndex] = key;
		Keypad_Refresh_Screen(keyBuffer);
		HAL_Delay(100);

		if(Keypad_Scan() == '6'){
			key = 'q';
			keyBuffer[bufferIndex] = key;
			Keypad_Refresh_Screen(keyBuffer);
			HAL_Delay(100);

			if(Keypad_Scan() == '6'){
				key = 'r';
				keyBuffer[bufferIndex] = key;
				Keypad_Refresh_Screen(keyBuffer);
				HAL_Delay(100);
			}
		}

		HAL_Delay(200);
	}
	else if(Keypad_Scan() == '7'){
		key = 's';
		keyBuffer[bufferIndex] = key;
		Keypad_Refresh_Screen(keyBuffer);
		HAL_Delay(100);

		if(Keypad_Scan() == '7'){
			key = 't';
			keyBuffer[bufferIndex] = key;
			Keypad_Refresh_Screen(keyBuffer);
			HAL_Delay(100);

			if(Keypad_Scan() == '7'){
				key = 'u';
				keyBuffer[bufferIndex] = key;
				Keypad_Refresh_Screen(keyBuffer);
				HAL_Delay(100);
			}
		}

		HAL_Delay(200);
	}
	else if(Keypad_Scan() == '8'){
		key = 'v';
		keyBuffer[bufferIndex] = key;
		Keypad_Refresh_Screen(keyBuffer);
		HAL_Delay(100);

		if(Keypad_Scan() == '8'){
			key = 'w';
			keyBuffer[bufferIndex] = key;
			Keypad_Refresh_Screen(keyBuffer);
			HAL_Delay(100);

			if(Keypad_Scan() == '8'){
				key = 'x';
				keyBuffer[bufferIndex] = key;
				Keypad_Refresh_Screen(keyBuffer);
				HAL_Delay(100);
			}
		}

		HAL_Delay(200);
	}
	else if(Keypad_Scan() == '9'){
		key = 'y';
		keyBuffer[bufferIndex] = key;
		Keypad_Refresh_Screen(keyBuffer);
		HAL_Delay(100);

		if(Keypad_Scan() == '9'){
			key = 'z';
			keyBuffer[bufferIndex] = key;
			Keypad_Refresh_Screen(keyBuffer);
			HAL_Delay(100);
		}

		HAL_Delay(200);
	}
	else if(Keypad_Scan() == '*'){
		key = '.';
		keyBuffer[bufferIndex] = key;
		Keypad_Refresh_Screen(keyBuffer);
		HAL_Delay(100);

		if(Keypad_Scan() == '*'){
			key = '!';
			keyBuffer[bufferIndex] = key;
			Keypad_Refresh_Screen(keyBuffer);
			HAL_Delay(100);

			if(Keypad_Scan() == '*'){
				key = '?';
				keyBuffer[bufferIndex] = key;
				Keypad_Refresh_Screen(keyBuffer);
				HAL_Delay(100);
			}
		}

		HAL_Delay(200);
	}
	else if(Keypad_Scan() == '0'){
		key = ' ';
		keyBuffer[bufferIndex] = key;
		Keypad_Refresh_Screen(keyBuffer);
		HAL_Delay(200);
	}
	else if((Keypad_Scan() == '#') && (bufferIndex > 0)){
		bufferIndex--;
		keyBuffer[bufferIndex] = '\0';
		Keypad_Refresh_Screen(keyBuffer);
		HAL_Delay(100);
	}

	if((key != '\0') && (bufferIndex < MESSAGE_LENGTH)){
		bufferIndex++;
	}
}

void Keypad_Refresh_Screen(char * keyBuffer) {
	lv_disp_load_scr(ui_enter_message_blank);
	lv_label_set_text(ui_Label56, keyBuffer);
	lv_disp_load_scr(ui_enter_message);
	lv_refr_now(NULL);
	HAL_Delay(200);
}


void Keypad_Clear_Buffer(void){
	memset(keyBuffer, 0, sizeof(keyBuffer));
	bufferIndex = 0;
}

