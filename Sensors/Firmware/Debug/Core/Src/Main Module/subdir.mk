################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Main\ Module/flash.c \
../Core/Src/Main\ Module/led.c 

OBJS += \
./Core/Src/Main\ Module/flash.o \
./Core/Src/Main\ Module/led.o 

C_DEPS += \
./Core/Src/Main\ Module/flash.d \
./Core/Src/Main\ Module/led.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Main\ Module/flash.o: ../Core/Src/Main\ Module/flash.c Core/Src/Main\ Module/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xE -c -I../Core/Inc -I../Core/Inc/Communication -I"../Core/Inc/Main Module" -I../Core/Inc/Sensors -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/Main Module/flash.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Main\ Module/led.o: ../Core/Src/Main\ Module/led.c Core/Src/Main\ Module/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xE -c -I../Core/Inc -I../Core/Inc/Communication -I"../Core/Inc/Main Module" -I../Core/Inc/Sensors -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/Main Module/led.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Main-20-Module

clean-Core-2f-Src-2f-Main-20-Module:
	-$(RM) ./Core/Src/Main\ Module/flash.d ./Core/Src/Main\ Module/flash.o ./Core/Src/Main\ Module/flash.su ./Core/Src/Main\ Module/led.d ./Core/Src/Main\ Module/led.o ./Core/Src/Main\ Module/led.su

.PHONY: clean-Core-2f-Src-2f-Main-20-Module

