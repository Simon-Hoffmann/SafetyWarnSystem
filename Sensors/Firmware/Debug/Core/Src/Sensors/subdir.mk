################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Sensors/door_sensor.c \
../Core/Src/Sensors/gas_sensor.c \
../Core/Src/Sensors/sensor.c \
../Core/Src/Sensors/water_sensor.c 

OBJS += \
./Core/Src/Sensors/door_sensor.o \
./Core/Src/Sensors/gas_sensor.o \
./Core/Src/Sensors/sensor.o \
./Core/Src/Sensors/water_sensor.o 

C_DEPS += \
./Core/Src/Sensors/door_sensor.d \
./Core/Src/Sensors/gas_sensor.d \
./Core/Src/Sensors/sensor.d \
./Core/Src/Sensors/water_sensor.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Sensors/%.o Core/Src/Sensors/%.su: ../Core/Src/Sensors/%.c Core/Src/Sensors/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xE -c -I../Core/Inc -I../Core/Inc/Communication -I"../Core/Inc/Main Module" -I../Core/Inc/Sensors -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Sensors

clean-Core-2f-Src-2f-Sensors:
	-$(RM) ./Core/Src/Sensors/door_sensor.d ./Core/Src/Sensors/door_sensor.o ./Core/Src/Sensors/door_sensor.su ./Core/Src/Sensors/gas_sensor.d ./Core/Src/Sensors/gas_sensor.o ./Core/Src/Sensors/gas_sensor.su ./Core/Src/Sensors/sensor.d ./Core/Src/Sensors/sensor.o ./Core/Src/Sensors/sensor.su ./Core/Src/Sensors/water_sensor.d ./Core/Src/Sensors/water_sensor.o ./Core/Src/Sensors/water_sensor.su

.PHONY: clean-Core-2f-Src-2f-Sensors

