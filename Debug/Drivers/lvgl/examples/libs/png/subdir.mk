################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/examples/libs/png/img_wink_png.c \
../Drivers/lvgl/examples/libs/png/lv_example_png_1.c 

OBJS += \
./Drivers/lvgl/examples/libs/png/img_wink_png.o \
./Drivers/lvgl/examples/libs/png/lv_example_png_1.o 

C_DEPS += \
./Drivers/lvgl/examples/libs/png/img_wink_png.d \
./Drivers/lvgl/examples/libs/png/lv_example_png_1.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/examples/libs/png/%.o Drivers/lvgl/examples/libs/png/%.su Drivers/lvgl/examples/libs/png/%.cyclo: ../Drivers/lvgl/examples/libs/png/%.c Drivers/lvgl/examples/libs/png/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4R5xx -c -I"C:/Users/Hunte/STM32CubeIDE/workspace_1.17.0/OLED_LVGL/Drivers/UI_files" -I../Core/Inc -I"C:/Users/Hunte/STM32CubeIDE/workspace_1.17.0/OLED_LVGL/Drivers/lvgl" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-examples-2f-libs-2f-png

clean-Drivers-2f-lvgl-2f-examples-2f-libs-2f-png:
	-$(RM) ./Drivers/lvgl/examples/libs/png/img_wink_png.cyclo ./Drivers/lvgl/examples/libs/png/img_wink_png.d ./Drivers/lvgl/examples/libs/png/img_wink_png.o ./Drivers/lvgl/examples/libs/png/img_wink_png.su ./Drivers/lvgl/examples/libs/png/lv_example_png_1.cyclo ./Drivers/lvgl/examples/libs/png/lv_example_png_1.d ./Drivers/lvgl/examples/libs/png/lv_example_png_1.o ./Drivers/lvgl/examples/libs/png/lv_example_png_1.su

.PHONY: clean-Drivers-2f-lvgl-2f-examples-2f-libs-2f-png

