################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../My_libs/Src/nextion.c \
../My_libs/Src/uart_lib.c 

OBJS += \
./My_libs/Src/nextion.o \
./My_libs/Src/uart_lib.o 

C_DEPS += \
./My_libs/Src/nextion.d \
./My_libs/Src/uart_lib.d 


# Each subdirectory must supply rules for building sources it contributes
My_libs/Src/%.o My_libs/Src/%.su: ../My_libs/Src/%.c My_libs/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F100xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-My_libs-2f-Src

clean-My_libs-2f-Src:
	-$(RM) ./My_libs/Src/nextion.d ./My_libs/Src/nextion.o ./My_libs/Src/nextion.su ./My_libs/Src/uart_lib.d ./My_libs/Src/uart_lib.o ./My_libs/Src/uart_lib.su

.PHONY: clean-My_libs-2f-Src

