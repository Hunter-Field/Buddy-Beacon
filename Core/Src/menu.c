#include "menu.h"

uint8_t up = 0;
uint8_t down = 0;
uint8_t right = 0;
uint8_t left = 0;
uint8_t enter = 0;

Menu_State state = MAIN_BEACONS;
//Menu_State state = ENTER_MESSAGE;
Menu_State selected_beacon = MAIN_BEACONS;

uint8_t curr_beacon = 0;

void displayMenu(Beacon *nearby_beacons){
    switch (state){
        case MAIN_BEACONS:

        	// CODE JUST TO SHOW OFF MENU
        	switch_screen(ui_main_beacons);
        	switch_screen(ui_main_waypoints);
        	switch_screen(ui_main_messages);
        	switch_screen(ui_tracking_n);
        	switch_screen(ui_view_message);
        	// END

//        	switch_screen(ui_main_beacons); // uncomment after demo
        	while(1){
        		if(enter){
					enter = 0;
					if(strcmp(nearby_beacons[1].name, "N/A") == 0){
						state = SUB_BEACON4;
					}
					else{
						state = SUB_BEACON1;
					}
					break;
				}
        		else if(right){
					right = 0;
					state = MAIN_WAYPOINTS;
					break;
				}
        	}
            break;
        case MAIN_WAYPOINTS:
        	switch_screen(ui_main_waypoints);
        	while(1){
        		if(enter){
					enter = 0;
//					state = SUB_WAYPOINT1;
					break;
				}
        		if(left){
					left = 0;
					state = MAIN_BEACONS;
					break;
				}
        		else if(right){
					right = 0;
					state = MAIN_MESSAGES;
					break;
				}
			}
            break;
        case MAIN_MESSAGES:
        	switch_screen(ui_main_messages);
        	if(enter){
				enter = 0;
//				state = SUB_MESSAGE4;
				break;
			}
			else if(left){
				left = 0;
				state = MAIN_WAYPOINTS;
				break;
			}
            break;

        //////////////////////////////////////////// BEACONS

        case SUB_BEACON1:
        	lv_label_set_text(ui_Label8, nearby_beacons[curr_beacon].name); // sub_beacon_1
			switch_screen(ui_sub_beacon1);
			while(1){
				if(up){
					up = 0;
					state = MAIN_BEACONS;
					selected_beacon = SUB_BEACON1;
					break;
				}
				else if(enter){
					enter = 0;
					state = TRACK_BEACON;
					selected_beacon = SUB_BEACON1;
					break;
				}
				else if(right){
					right = 0;
					state = SUB_BEACON2;
					selected_beacon = SUB_BEACON1;
					curr_beacon++;
					break;
				}
			}
			break;
        case SUB_BEACON2:
			lv_label_set_text(ui_Label10, nearby_beacons[curr_beacon].name); // sub_beacon_2
			switch_screen(ui_sub_beacon2);
			while(1){
				if(up){
					up = 0;
					state = MAIN_BEACONS;
					selected_beacon = SUB_BEACON2;
					break;
				}
				else if(enter){
					enter = 0;
					state = TRACK_BEACON;
					selected_beacon = SUB_BEACON2;
					break;
				}
				else if(left){
					left = 0;
					if(curr_beacon == 1){
						state = SUB_BEACON1;
						selected_beacon = SUB_BEACON2;
					}
					else{
						state = SUB_BEACON2;
						selected_beacon = SUB_BEACON2;
					}
					curr_beacon--;
					break;
				}
				else if(right){
					right = 0;
					if(curr_beacon == NUM_NEARBY - 1){
						state = SUB_BEACON3;
						selected_beacon = SUB_BEACON2;
					}
					else{
						state = SUB_BEACON2;
						selected_beacon = SUB_BEACON2;
					}
					state = SUB_BEACON2;
					curr_beacon++;
					break;
				}
			}
			break;
        case SUB_BEACON3:
			lv_label_set_text(ui_Label36, nearby_beacons[curr_beacon].name); // sub_beacon_3
			switch_screen(ui_sub_beacon3);
			while(1){
				if(up){
					up = 0;
					state = MAIN_BEACONS;
					selected_beacon = SUB_BEACON3;
					break;
				}
				else if(enter){
					enter = 0;
					state = TRACK_BEACON;
					selected_beacon = SUB_BEACON3;
					break;
				}
				else if(left){
					left = 0;
					state = SUB_BEACON2;
					selected_beacon = SUB_BEACON3;
					curr_beacon--;
					break;
				}
			}
			break;
        case SUB_BEACON4:
        	lv_label_set_text(ui_Label83, nearby_beacons[curr_beacon].name); // sub_beacon_4
        	switch_screen(ui_sub_beacon4);
        	while(1){
				if(up){
					up = 0;
					state = MAIN_BEACONS;
					selected_beacon = SUB_BEACON4;
					break;
				}
				else if(enter && strcmp(nearby_beacons[1].name, "N/A") == 0){
					enter = 0;
					state = TRACK_BEACON;
					selected_beacon = SUB_BEACON4;
					break;
				}
        	}
			break;

        case TRACK_BEACON:
			switch_screen(ui_track_beacon);
			while(1){
				if(up){
					up = 0;
					state = selected_beacon;
					break;
				}
				if(right){
					right = 0;
					state = MESSAGE_BEACON;
					break;
				}
				else if(enter){
					enter = 0;
					state = TRACKING_N;
					break;
				}
			}
			break;
        case TRACKING_N:
			switch_screen(ui_track_beacon);
			while(1){
				if(up){
					up = 0;
					state = selected_beacon;
					break;
				}
				if(right){
					right = 0;
					state = MESSAGE_BEACON;
					break;
				}
				else if(enter){
					enter = 0;
					state = TRACKING_N;
					break;
				}
			}
			break;

        case MESSAGE_BEACON:
			switch_screen(ui_message_beacon);
			while(1){
				if(up){
					up = 0;
					state = selected_beacon;
					break;
				}
				if(left){
					right = 0;
					state = TRACK_BEACON;
					break;
				}
				else if(enter){
					enter = 0;
					state = ENTER_MESSAGE;
					break;
				}
			}
			break;
        case ENTER_MESSAGE:
			switch_screen(ui_enter_message);
			while(1){
				while (!up && !enter) {
					Keypad_Update();

					if(up){
						up = 0;
						state = MESSAGE_BEACON;
						Keypad_Clear_Buffer();
						break;
					}
					else if(enter){
						enter = 0;
						// SEND MESSAGE OVER LORA
//						HAL_UART_Transmit(&huart2, message, sizeof(message), 100);
						break;
					}
				}
			}
			break;
		//////////////////////////////////////////// WAYPOINTS
    }
}

void switch_screen(lv_obj_t * screen) {
	lv_disp_load_scr(screen);
	lv_refr_now(NULL);
	HAL_Delay(200);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_15)
    {
        up = 1;
    }

    if (GPIO_Pin == GPIO_PIN_1)
	{
		right = 1;
	}

    if (GPIO_Pin == GPIO_PIN_2)
	{
		left = 1;
	}

    if (GPIO_Pin == GPIO_PIN_10)
	{
		enter = 1;
	}
}


// right 37 - PB2
// left 17 - PC2
// middle 47 - PB10
// down 16 - PC1
// up 15 - PC0
