################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LED_Functions/LED_OUTPUT.c 

C_DEPS += \
./Core/Src/LED_Functions/LED_OUTPUT.d 

OBJS += \
./Core/Src/LED_Functions/LED_OUTPUT.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/LED_Functions/LED_OUTPUT.o: ../Core/Src/LED_Functions/LED_OUTPUT.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/LED_Functions/LED_OUTPUT.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

