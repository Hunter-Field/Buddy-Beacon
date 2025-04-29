SET(SOURCES screens/ui_main_beacons.c
    screens/ui_sub_beacon1.c
    screens/ui_sub_beacon2.c
    screens/ui_sub_beacon3.c
    screens/ui_sub_beacon4.c
    screens/ui_track_beacon.c
    screens/ui_tracking_n.c
    screens/ui_tracking_e.c
    screens/ui_tracking_s.c
    screens/ui_tracking_w.c
    screens/ui_message_beacon.c
    screens/ui_enter_message.c
    screens/ui_main_waypoints.c
    screens/ui_sub_waypoint1.c
    screens/ui_sub_waypoint2.c
    screens/ui_sub_waypoint3.c
    screens/ui_track_waypoint.c
    screens/ui_del_waypoint.c
    screens/ui_add_waypoint.c
    screens/ui_main_messages.c
    screens/ui_sub_message1.c
    screens/ui_sub_message3.c
    screens/ui_sub_message2.c
    screens/ui_sub_message4.c
    screens/ui_view_message.c
    screens/ui_enter_message_blank.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c)

add_library(ui ${SOURCES})
