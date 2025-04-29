################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/UI_files/components/ui_comp_hook.c 

OBJS += \
./Drivers/UI_files/components/ui_comp_hook.o 

C_DEPS += \
./Drivers/UI_files/components/ui_comp_hook.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/UI_files/components/%.o Drivers/UI_files/components/%.su Drivers/UI_files/components/%.cyclo: ../Drivers/UI_files/components/%.c Drivers/UI_files/components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4R5xx -c -I"C:/Users/Hunte/STM32CubeIDE/workspace_1.17.0/OLED_LVGL/Drivers/UI_files" -I../Core/Inc -I"C:/Users/Hunte/STM32CubeIDE/workspace_1.17.0/OLED_LVGL/Drivers/lvgl" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-UI_files-2f-components

clean-Drivers-2f-UI_files-2f-components:
	-$(RM) ./Drivers/UI_files/components/ui_comp_hook.cyclo ./Drivers/UI_files/components/ui_comp_hook.d ./Drivers/UI_files/components/ui_comp_hook.o ./Drivers/UI_files/components/ui_comp_hook.su

.PHONY: clean-Drivers-2f-UI_files-2f-components

