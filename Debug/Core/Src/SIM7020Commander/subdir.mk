################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/SIM7020Commander/SIM7020HTTP.c 

C_DEPS += \
./Core/Src/SIM7020Commander/SIM7020HTTP.d 

OBJS += \
./Core/Src/SIM7020Commander/SIM7020HTTP.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/SIM7020Commander/SIM7020HTTP.o: ../Core/Src/SIM7020Commander/SIM7020HTTP.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/SIM7020Commander/SIM7020HTTP.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

