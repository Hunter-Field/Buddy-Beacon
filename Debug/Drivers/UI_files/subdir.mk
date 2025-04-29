################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/UI_files/ui.c \
../Drivers/UI_files/ui_helpers.c 

OBJS += \
./Drivers/UI_files/ui.o \
./Drivers/UI_files/ui_helpers.o 

C_DEPS += \
./Drivers/UI_files/ui.d \
./Drivers/UI_files/ui_helpers.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/UI_files/%.o Drivers/UI_files/%.su Drivers/UI_files/%.cyclo: ../Drivers/UI_files/%.c Drivers/UI_files/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4R5xx -c -I"C:/Users/Hunte/STM32CubeIDE/workspace_1.17.0/OLED_LVGL/Drivers/UI_files" -I../Core/Inc -I"C:/Users/Hunte/STM32CubeIDE/workspace_1.17.0/OLED_LVGL/Drivers/lvgl" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-UI_files

clean-Drivers-2f-UI_files:
	-$(RM) ./Drivers/UI_files/ui.cyclo ./Drivers/UI_files/ui.d ./Drivers/UI_files/ui.o ./Drivers/UI_files/ui.su ./Drivers/UI_files/ui_helpers.cyclo ./Drivers/UI_files/ui_helpers.d ./Drivers/UI_files/ui_helpers.o ./Drivers/UI_files/ui_helpers.su

.PHONY: clean-Drivers-2f-UI_files

