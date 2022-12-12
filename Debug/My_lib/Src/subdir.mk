################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../My_lib/Src/nextion.c \
../My_lib/Src/uart_lib.c 

OBJS += \
./My_lib/Src/nextion.o \
./My_lib/Src/uart_lib.o 

C_DEPS += \
./My_lib/Src/nextion.d \
./My_lib/Src/uart_lib.d 


# Each subdirectory must supply rules for building sources it contributes
My_lib/Src/%.o My_lib/Src/%.su: ../My_lib/Src/%.c My_lib/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F100xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../My_lib/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-My_lib-2f-Src

clean-My_lib-2f-Src:
	-$(RM) ./My_lib/Src/nextion.d ./My_lib/Src/nextion.o ./My_lib/Src/nextion.su ./My_lib/Src/uart_lib.d ./My_lib/Src/uart_lib.o ./My_lib/Src/uart_lib.su

.PHONY: clean-My_lib-2f-Src

