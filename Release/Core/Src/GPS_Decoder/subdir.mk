################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/GPS_Decoder/GPSdecode.c 

C_DEPS += \
./Core/Src/GPS_Decoder/GPSdecode.d 

OBJS += \
./Core/Src/GPS_Decoder/GPSdecode.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/GPS_Decoder/GPSdecode.o: ../Core/Src/GPS_Decoder/GPSdecode.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/GPS_Decoder/GPSdecode.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

