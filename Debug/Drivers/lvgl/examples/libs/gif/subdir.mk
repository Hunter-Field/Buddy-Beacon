################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/examples/libs/gif/img_bulb_gif.c \
../Drivers/lvgl/examples/libs/gif/lv_example_gif_1.c 

OBJS += \
./Drivers/lvgl/examples/libs/gif/img_bulb_gif.o \
./Drivers/lvgl/examples/libs/gif/lv_example_gif_1.o 

C_DEPS += \
./Drivers/lvgl/examples/libs/gif/img_bulb_gif.d \
./Drivers/lvgl/examples/libs/gif/lv_example_gif_1.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/examples/libs/gif/%.o Drivers/lvgl/examples/libs/gif/%.su Drivers/lvgl/examples/libs/gif/%.cyclo: ../Drivers/lvgl/examples/libs/gif/%.c Drivers/lvgl/examples/libs/gif/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4R5xx -c -I"C:/Users/Hunte/STM32CubeIDE/workspace_1.17.0/OLED_LVGL/Drivers/UI_files" -I../Core/Inc -I"C:/Users/Hunte/STM32CubeIDE/workspace_1.17.0/OLED_LVGL/Drivers/lvgl" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-examples-2f-libs-2f-gif

clean-Drivers-2f-lvgl-2f-examples-2f-libs-2f-gif:
	-$(RM) ./Drivers/lvgl/examples/libs/gif/img_bulb_gif.cyclo ./Drivers/lvgl/examples/libs/gif/img_bulb_gif.d ./Drivers/lvgl/examples/libs/gif/img_bulb_gif.o ./Drivers/lvgl/examples/libs/gif/img_bulb_gif.su ./Drivers/lvgl/examples/libs/gif/lv_example_gif_1.cyclo ./Drivers/lvgl/examples/libs/gif/lv_example_gif_1.d ./Drivers/lvgl/examples/libs/gif/lv_example_gif_1.o ./Drivers/lvgl/examples/libs/gif/lv_example_gif_1.su

.PHONY: clean-Drivers-2f-lvgl-2f-examples-2f-libs-2f-gif

