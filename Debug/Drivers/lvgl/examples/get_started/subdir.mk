################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/examples/get_started/lv_example_get_started_1.c \
../Drivers/lvgl/examples/get_started/lv_example_get_started_2.c \
../Drivers/lvgl/examples/get_started/lv_example_get_started_3.c 

OBJS += \
./Drivers/lvgl/examples/get_started/lv_example_get_started_1.o \
./Drivers/lvgl/examples/get_started/lv_example_get_started_2.o \
./Drivers/lvgl/examples/get_started/lv_example_get_started_3.o 

C_DEPS += \
./Drivers/lvgl/examples/get_started/lv_example_get_started_1.d \
./Drivers/lvgl/examples/get_started/lv_example_get_started_2.d \
./Drivers/lvgl/examples/get_started/lv_example_get_started_3.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/examples/get_started/%.o Drivers/lvgl/examples/get_started/%.su Drivers/lvgl/examples/get_started/%.cyclo: ../Drivers/lvgl/examples/get_started/%.c Drivers/lvgl/examples/get_started/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4R5xx -c -I"C:/Users/Hunte/STM32CubeIDE/workspace_1.17.0/OLED_LVGL/Drivers/UI_files" -I../Core/Inc -I"C:/Users/Hunte/STM32CubeIDE/workspace_1.17.0/OLED_LVGL/Drivers/lvgl" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-examples-2f-get_started

clean-Drivers-2f-lvgl-2f-examples-2f-get_started:
	-$(RM) ./Drivers/lvgl/examples/get_started/lv_example_get_started_1.cyclo ./Drivers/lvgl/examples/get_started/lv_example_get_started_1.d ./Drivers/lvgl/examples/get_started/lv_example_get_started_1.o ./Drivers/lvgl/examples/get_started/lv_example_get_started_1.su ./Drivers/lvgl/examples/get_started/lv_example_get_started_2.cyclo ./Drivers/lvgl/examples/get_started/lv_example_get_started_2.d ./Drivers/lvgl/examples/get_started/lv_example_get_started_2.o ./Drivers/lvgl/examples/get_started/lv_example_get_started_2.su ./Drivers/lvgl/examples/get_started/lv_example_get_started_3.cyclo ./Drivers/lvgl/examples/get_started/lv_example_get_started_3.d ./Drivers/lvgl/examples/get_started/lv_example_get_started_3.o ./Drivers/lvgl/examples/get_started/lv_example_get_started_3.su

.PHONY: clean-Drivers-2f-lvgl-2f-examples-2f-get_started

