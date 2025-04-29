#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "stm32l4xx_hal.h"
#include "LVGL_drivers.h"

#define MESSAGE_LENGTH 200

char Keypad_Scan(void);
void Keypad_Update(void);
void Keypad_Refresh_Screen(char * keyBuffer);
void Keypad_Clear_Buffer(void);

#endif /* INC_KEYPAD_H_ */
