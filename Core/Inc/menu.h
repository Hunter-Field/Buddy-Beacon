#ifndef INC_MENU_H_
#define INC_MENU_H_

#include "keypad.h"

typedef enum {
    MAIN_BEACONS,
    MAIN_WAYPOINTS,
    MAIN_MESSAGES,
	SUB_BEACON1,
	SUB_BEACON2,
	SUB_BEACON3,
	SUB_BEACON4,
	TRACK_BEACON,
	TRACKING_N,
	MESSAGE_BEACON,
	ENTER_MESSAGE
} Menu_State;

void displayMenu(Beacon *nearby_beacons);
void switch_screen(lv_obj_t * screen);

#endif /* INC_MENU_H_ */
