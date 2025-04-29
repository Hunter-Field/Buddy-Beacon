################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/UI_files/screens/ui_add_waypoint.c \
../Drivers/UI_files/screens/ui_del_waypoint.c \
../Drivers/UI_files/screens/ui_enter_message.c \
../Drivers/UI_files/screens/ui_enter_message_blank.c \
../Drivers/UI_files/screens/ui_main_beacons.c \
../Drivers/UI_files/screens/ui_main_messages.c \
../Drivers/UI_files/screens/ui_main_waypoints.c \
../Drivers/UI_files/screens/ui_message_beacon.c \
../Drivers/UI_files/screens/ui_sub_beacon1.c \
../Drivers/UI_files/screens/ui_sub_beacon2.c \
../Drivers/UI_files/screens/ui_sub_beacon3.c \
../Drivers/UI_files/screens/ui_sub_beacon4.c \
../Drivers/UI_files/screens/ui_sub_message1.c \
../Drivers/UI_files/screens/ui_sub_message2.c \
../Drivers/UI_files/screens/ui_sub_message3.c \
../Drivers/UI_files/screens/ui_sub_message4.c \
../Drivers/UI_files/screens/ui_sub_waypoint1.c \
../Drivers/UI_files/screens/ui_sub_waypoint2.c \
../Drivers/UI_files/screens/ui_sub_waypoint3.c \
../Drivers/UI_files/screens/ui_track_beacon.c \
../Drivers/UI_files/screens/ui_track_waypoint.c \
../Drivers/UI_files/screens/ui_tracking_e.c \
../Drivers/UI_files/screens/ui_tracking_n.c \
../Drivers/UI_files/screens/ui_tracking_s.c \
../Drivers/UI_files/screens/ui_tracking_w.c \
../Drivers/UI_files/screens/ui_view_message.c 

OBJS += \
./Drivers/UI_files/screens/ui_add_waypoint.o \
./Drivers/UI_files/screens/ui_del_waypoint.o \
./Drivers/UI_files/screens/ui_enter_message.o \
./Drivers/UI_files/screens/ui_enter_message_blank.o \
./Drivers/UI_files/screens/ui_main_beacons.o \
./Drivers/UI_files/screens/ui_main_messages.o \
./Drivers/UI_files/screens/ui_main_waypoints.o \
./Drivers/UI_files/screens/ui_message_beacon.o \
./Drivers/UI_files/screens/ui_sub_beacon1.o \
./Drivers/UI_files/screens/ui_sub_beacon2.o \
./Drivers/UI_files/screens/ui_sub_beacon3.o \
./Drivers/UI_files/screens/ui_sub_beacon4.o \
./Drivers/UI_files/screens/ui_sub_message1.o \
./Drivers/UI_files/screens/ui_sub_message2.o \
./Drivers/UI_files/screens/ui_sub_message3.o \
./Drivers/UI_files/screens/ui_sub_message4.o \
./Drivers/UI_files/screens/ui_sub_waypoint1.o \
./Drivers/UI_files/screens/ui_sub_waypoint2.o \
./Drivers/UI_files/screens/ui_sub_waypoint3.o \
./Drivers/UI_files/screens/ui_track_beacon.o \
./Drivers/UI_files/screens/ui_track_waypoint.o \
./Drivers/UI_files/screens/ui_tracking_e.o \
./Drivers/UI_files/screens/ui_tracking_n.o \
./Drivers/UI_files/screens/ui_tracking_s.o \
./Drivers/UI_files/screens/ui_tracking_w.o \
./Drivers/UI_files/screens/ui_view_message.o 

C_DEPS += \
./Drivers/UI_files/screens/ui_add_waypoint.d \
./Drivers/UI_files/screens/ui_del_waypoint.d \
./Drivers/UI_files/screens/ui_enter_message.d \
./Drivers/UI_files/screens/ui_enter_message_blank.d \
./Drivers/UI_files/screens/ui_main_beacons.d \
./Drivers/UI_files/screens/ui_main_messages.d \
./Drivers/UI_files/screens/ui_main_waypoints.d \
./Drivers/UI_files/screens/ui_message_beacon.d \
./Drivers/UI_files/screens/ui_sub_beacon1.d \
./Drivers/UI_files/screens/ui_sub_beacon2.d \
./Drivers/UI_files/screens/ui_sub_beacon3.d \
./Drivers/UI_files/screens/ui_sub_beacon4.d \
./Drivers/UI_files/screens/ui_sub_message1.d \
./Drivers/UI_files/screens/ui_sub_message2.d \
./Drivers/UI_files/screens/ui_sub_message3.d \
./Drivers/UI_files/screens/ui_sub_message4.d \
./Drivers/UI_files/screens/ui_sub_waypoint1.d \
./Drivers/UI_files/screens/ui_sub_waypoint2.d \
./Drivers/UI_files/screens/ui_sub_waypoint3.d \
./Drivers/UI_files/screens/ui_track_beacon.d \
./Drivers/UI_files/screens/ui_track_waypoint.d \
./Drivers/UI_files/screens/ui_tracking_e.d \
./Drivers/UI_files/screens/ui_tracking_n.d \
./Drivers/UI_files/screens/ui_tracking_s.d \
./Drivers/UI_files/screens/ui_tracking_w.d \
./Drivers/UI_files/screens/ui_view_message.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/UI_files/screens/%.o Drivers/UI_files/screens/%.su Drivers/UI_files/screens/%.cyclo: ../Drivers/UI_files/screens/%.c Drivers/UI_files/screens/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4R5xx -c -I"C:/Users/Hunte/STM32CubeIDE/workspace_1.17.0/OLED_LVGL/Drivers/UI_files" -I../Core/Inc -I"C:/Users/Hunte/STM32CubeIDE/workspace_1.17.0/OLED_LVGL/Drivers/lvgl" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-UI_files-2f-screens

clean-Drivers-2f-UI_files-2f-screens:
	-$(RM) ./Drivers/UI_files/screens/ui_add_waypoint.cyclo ./Drivers/UI_files/screens/ui_add_waypoint.d ./Drivers/UI_files/screens/ui_add_waypoint.o ./Drivers/UI_files/screens/ui_add_waypoint.su ./Drivers/UI_files/screens/ui_del_waypoint.cyclo ./Drivers/UI_files/screens/ui_del_waypoint.d ./Drivers/UI_files/screens/ui_del_waypoint.o ./Drivers/UI_files/screens/ui_del_waypoint.su ./Drivers/UI_files/screens/ui_enter_message.cyclo ./Drivers/UI_files/screens/ui_enter_message.d ./Drivers/UI_files/screens/ui_enter_message.o ./Drivers/UI_files/screens/ui_enter_message.su ./Drivers/UI_files/screens/ui_enter_message_blank.cyclo ./Drivers/UI_files/screens/ui_enter_message_blank.d ./Drivers/UI_files/screens/ui_enter_message_blank.o ./Drivers/UI_files/screens/ui_enter_message_blank.su ./Drivers/UI_files/screens/ui_main_beacons.cyclo ./Drivers/UI_files/screens/ui_main_beacons.d ./Drivers/UI_files/screens/ui_main_beacons.o ./Drivers/UI_files/screens/ui_main_beacons.su ./Drivers/UI_files/screens/ui_main_messages.cyclo ./Drivers/UI_files/screens/ui_main_messages.d ./Drivers/UI_files/screens/ui_main_messages.o ./Drivers/UI_files/screens/ui_main_messages.su ./Drivers/UI_files/screens/ui_main_waypoints.cyclo ./Drivers/UI_files/screens/ui_main_waypoints.d ./Drivers/UI_files/screens/ui_main_waypoints.o ./Drivers/UI_files/screens/ui_main_waypoints.su ./Drivers/UI_files/screens/ui_message_beacon.cyclo ./Drivers/UI_files/screens/ui_message_beacon.d ./Drivers/UI_files/screens/ui_message_beacon.o ./Drivers/UI_files/screens/ui_message_beacon.su ./Drivers/UI_files/screens/ui_sub_beacon1.cyclo ./Drivers/UI_files/screens/ui_sub_beacon1.d ./Drivers/UI_files/screens/ui_sub_beacon1.o ./Drivers/UI_files/screens/ui_sub_beacon1.su ./Drivers/UI_files/screens/ui_sub_beacon2.cyclo ./Drivers/UI_files/screens/ui_sub_beacon2.d ./Drivers/UI_files/screens/ui_sub_beacon2.o ./Drivers/UI_files/screens/ui_sub_beacon2.su ./Drivers/UI_files/screens/ui_sub_beacon3.cyclo ./Drivers/UI_files/screens/ui_sub_beacon3.d ./Drivers/UI_files/screens/ui_sub_beacon3.o ./Drivers/UI_files/screens/ui_sub_beacon3.su ./Drivers/UI_files/screens/ui_sub_beacon4.cyclo ./Drivers/UI_files/screens/ui_sub_beacon4.d ./Drivers/UI_files/screens/ui_sub_beacon4.o ./Drivers/UI_files/screens/ui_sub_beacon4.su ./Drivers/UI_files/screens/ui_sub_message1.cyclo ./Drivers/UI_files/screens/ui_sub_message1.d ./Drivers/UI_files/screens/ui_sub_message1.o ./Drivers/UI_files/screens/ui_sub_message1.su ./Drivers/UI_files/screens/ui_sub_message2.cyclo ./Drivers/UI_files/screens/ui_sub_message2.d ./Drivers/UI_files/screens/ui_sub_message2.o ./Drivers/UI_files/screens/ui_sub_message2.su ./Drivers/UI_files/screens/ui_sub_message3.cyclo ./Drivers/UI_files/screens/ui_sub_message3.d ./Drivers/UI_files/screens/ui_sub_message3.o ./Drivers/UI_files/screens/ui_sub_message3.su ./Drivers/UI_files/screens/ui_sub_message4.cyclo ./Drivers/UI_files/screens/ui_sub_message4.d ./Drivers/UI_files/screens/ui_sub_message4.o ./Drivers/UI_files/screens/ui_sub_message4.su ./Drivers/UI_files/screens/ui_sub_waypoint1.cyclo ./Drivers/UI_files/screens/ui_sub_waypoint1.d ./Drivers/UI_files/screens/ui_sub_waypoint1.o ./Drivers/UI_files/screens/ui_sub_waypoint1.su ./Drivers/UI_files/screens/ui_sub_waypoint2.cyclo ./Drivers/UI_files/screens/ui_sub_waypoint2.d ./Drivers/UI_files/screens/ui_sub_waypoint2.o ./Drivers/UI_files/screens/ui_sub_waypoint2.su ./Drivers/UI_files/screens/ui_sub_waypoint3.cyclo ./Drivers/UI_files/screens/ui_sub_waypoint3.d ./Drivers/UI_files/screens/ui_sub_waypoint3.o ./Drivers/UI_files/screens/ui_sub_waypoint3.su ./Drivers/UI_files/screens/ui_track_beacon.cyclo ./Drivers/UI_files/screens/ui_track_beacon.d ./Drivers/UI_files/screens/ui_track_beacon.o ./Drivers/UI_files/screens/ui_track_beacon.su ./Drivers/UI_files/screens/ui_track_waypoint.cyclo ./Drivers/UI_files/screens/ui_track_waypoint.d ./Drivers/UI_files/screens/ui_track_waypoint.o ./Drivers/UI_files/screens/ui_track_waypoint.su ./Drivers/UI_files/screens/ui_tracking_e.cyclo ./Drivers/UI_files/screens/ui_tracking_e.d ./Drivers/UI_files/screens/ui_tracking_e.o ./Drivers/UI_files/screens/ui_tracking_e.su ./Drivers/UI_files/screens/ui_tracking_n.cyclo ./Drivers/UI_files/screens/ui_tracking_n.d ./Drivers/UI_files/screens/ui_tracking_n.o ./Drivers/UI_files/screens/ui_tracking_n.su ./Drivers/UI_files/screens/ui_tracking_s.cyclo ./Drivers/UI_files/screens/ui_tracking_s.d ./Drivers/UI_files/screens/ui_tracking_s.o ./Drivers/UI_files/screens/ui_tracking_s.su ./Drivers/UI_files/screens/ui_tracking_w.cyclo ./Drivers/UI_files/screens/ui_tracking_w.d ./Drivers/UI_files/screens/ui_tracking_w.o ./Drivers/UI_files/screens/ui_tracking_w.su ./Drivers/UI_files/screens/ui_view_message.cyclo ./Drivers/UI_files/screens/ui_view_message.d ./Drivers/UI_files/screens/ui_view_message.o ./Drivers/UI_files/screens/ui_view_message.su

.PHONY: clean-Drivers-2f-UI_files-2f-screens

