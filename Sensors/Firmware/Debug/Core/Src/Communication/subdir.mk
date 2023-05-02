################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Communication/RFM69W.c \
../Core/Src/Communication/SPI.c \
../Core/Src/Communication/packet.c \
../Core/Src/Communication/rfm_hal.c 

OBJS += \
./Core/Src/Communication/RFM69W.o \
./Core/Src/Communication/SPI.o \
./Core/Src/Communication/packet.o \
./Core/Src/Communication/rfm_hal.o 

C_DEPS += \
./Core/Src/Communication/RFM69W.d \
./Core/Src/Communication/SPI.d \
./Core/Src/Communication/packet.d \
./Core/Src/Communication/rfm_hal.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Communication/%.o Core/Src/Communication/%.su: ../Core/Src/Communication/%.c Core/Src/Communication/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xE -c -I../Core/Inc -I../Core/Inc/Communication -I"../Core/Inc/Main Module" -I../Core/Inc/Sensors -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Communication

clean-Core-2f-Src-2f-Communication:
	-$(RM) ./Core/Src/Communication/RFM69W.d ./Core/Src/Communication/RFM69W.o ./Core/Src/Communication/RFM69W.su ./Core/Src/Communication/SPI.d ./Core/Src/Communication/SPI.o ./Core/Src/Communication/SPI.su ./Core/Src/Communication/packet.d ./Core/Src/Communication/packet.o ./Core/Src/Communication/packet.su ./Core/Src/Communication/rfm_hal.d ./Core/Src/Communication/rfm_hal.o ./Core/Src/Communication/rfm_hal.su

.PHONY: clean-Core-2f-Src-2f-Communication

